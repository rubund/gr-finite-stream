/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/buffer.h>
#include <boost/thread.hpp>
#include "repeat_stream_impl.h"

namespace gr {
  namespace finite_stream {

    repeat_stream::sptr
    repeat_stream::make(size_t itemsize, int n, int max_samples)
    {
      return gnuradio::get_initial_sptr
        (new repeat_stream_impl(itemsize, n, max_samples));
    }

    /*
     * The private constructor
     */
    repeat_stream_impl::repeat_stream_impl(size_t itemsize, int n, int max_samples)
      : gr::block("repeat_stream",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make(1, 1, itemsize)),
        d_itemsize(itemsize), d_n(n), d_max_samples(max_samples),
        d_state(0), d_memory_cnt(0), d_playback_cnt(0), d_playback_time(0),
        d_input_done(false)
    {
      d_pagesize = pagesize();
      d_memory_pointers.push_back((char*)malloc(d_pagesize));
      d_max_n_memory_pages = (int)ceil(((float)(max_samples * d_itemsize))/((float)d_pagesize));


      set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    repeat_stream_impl::~repeat_stream_impl()
    {
      for (int i=0; i < d_memory_pointers.size(); i++) {
        free(d_memory_pointers[i]);
      }
      d_memory_pointers.clear();
    }

#if defined(_WIN32) && defined(HAVE_GETPAGESIZE)
  extern "C" size_t getpagesize(void);
#endif

    int
    repeat_stream_impl::pagesize()
    {
      static int s_pagesize = -1;
 
      if(s_pagesize == -1) {
#if defined(HAVE_GETPAGESIZE)
        s_pagesize = getpagesize();
#elif defined (HAVE_SYSCONF)
        s_pagesize = sysconf(_SC_PAGESIZE);
        if(s_pagesize == -1) {
          perror("_SC_PAGESIZE");
          s_pagesize = 4096;
        }
#else
        fprintf(stderr, "gr::pagesize: no info; setting pagesize = 4096\n");
        s_pagesize = 4096;
#endif
     }
     return s_pagesize;

    }

    bool
    repeat_stream_impl::start()
    {   
      d_block_detail = this->detail().get();
      return true;
    }

    void
    repeat_stream_impl::copy_to_memory(int pos, const char *src, int length)
    {
      const char *current_srcpos = src;
      int current_pos = pos;
      int ncopy;
      int page_index;
      int page_local_number;
      int remaining_length = length;
      page_index = current_pos / d_pagesize;
      page_local_number = current_pos % d_pagesize;

      while(remaining_length > 0) {
        while(d_memory_pointers.size() < (page_index+1)) {
          if (d_memory_pointers.size() >= d_max_n_memory_pages) {
            throw std::runtime_error("Tried writing to memory outside the bounds specified by max_samples");
          }
          d_memory_pointers.push_back((char*)malloc(d_pagesize));
        }

        if (remaining_length > (d_pagesize-page_local_number))
            ncopy = d_pagesize - page_local_number;
        else
            ncopy = remaining_length;

        memcpy(d_memory_pointers[page_index]+page_local_number, current_srcpos, ncopy);

        remaining_length = remaining_length - ncopy;
        current_pos = current_pos + ncopy;
        page_index = current_pos / d_pagesize;
        page_local_number = current_pos % d_pagesize;
        current_srcpos = current_srcpos + ncopy;
      }
    }

    void
    repeat_stream_impl::copy_from_memory(char *dst, int pos, int length)
    {
      char *current_dstpos = dst;
      int current_pos = pos;
      int ncopy;
      int page_index;
      int page_local_number;
      int remaining_length = length;
      page_index = current_pos / d_pagesize;
      page_local_number = current_pos % d_pagesize;

      while(remaining_length > 0) {
        if (page_index > (d_memory_pointers.size() - 1)) {
          throw std::runtime_error("Tried reading from memory not yet allocated");
        }

        if (remaining_length > (d_pagesize-page_local_number))
            ncopy = d_pagesize - page_local_number;
        else
            ncopy = remaining_length;

        memcpy(current_dstpos, d_memory_pointers[page_index]+page_local_number, ncopy);

        remaining_length = remaining_length - ncopy;
        current_pos = current_pos + ncopy;
        page_index = current_pos / d_pagesize;
        page_local_number = current_pos % d_pagesize;
        current_dstpos = current_dstpos + ncopy;
      }
    }

    void
    repeat_stream_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      int ninput_items;
      bool input_done;
      int current_ninput_items_required;

      if (d_state == 0) {
        gr::thread::scoped_lock guard(*d_block_detail->input(0)->mutex());
        ninput_items = d_block_detail->input(0)->items_available ();
        input_done = d_block_detail->input(0)->done();

        if (input_done) {
          d_input_done = true;
          d_remaining_input = ninput_items;
          ninput_items_required[0] = 0;
          return;
        }
      }

      if (d_state == 0) {
        ninput_items_required[0] = noutput_items;
      }
      else {
        ninput_items_required[0] = 0;
      }
    }

    int
    repeat_stream_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];

      int produced = 0;
      int consumed = 0;

      if (d_n == 1 && d_input_done && ninput_items[0] == 0) {
        return -1;
      }

      if (d_state == 0) {
        int minelem = std::min(noutput_items, ninput_items[0]);

        if ((d_memory_cnt + minelem) <= d_max_samples) {
          copy_to_memory((d_memory_cnt * d_itemsize), in, d_itemsize * minelem);
          memcpy(out, in, d_itemsize * minelem);
          d_memory_cnt += minelem;
          produced = minelem;
          consumed = minelem;
        }
        else {
          if(d_max_samples > d_memory_cnt)
              copy_to_memory((d_memory_cnt * d_itemsize), in, d_itemsize * (d_max_samples - d_memory_cnt));
          int minelem2 = std::min((d_max_samples - d_memory_cnt), minelem);
          memcpy(out, in, d_itemsize * minelem2);
          produced = minelem2;
          d_memory_cnt = d_max_samples;
          consumed = ninput_items[0];
        }
      }
      else if (d_state == 1) {
        if ((d_playback_cnt + noutput_items) <= d_memory_cnt) {
          copy_from_memory(out, (d_playback_cnt * d_itemsize), d_itemsize * noutput_items);
          d_playback_cnt += noutput_items;
          produced = noutput_items;
          consumed = 0;
        }
        else {
          copy_from_memory(out, (d_playback_cnt * d_itemsize), d_itemsize * (d_memory_cnt - d_playback_cnt));
          produced = d_memory_cnt - d_playback_cnt;
          consumed = 0;
          d_playback_cnt = 0;
          if (d_n > 1) { // If 0, it loops forever. If 1, it returns from this function earlier.
            d_playback_time++;
            if ((d_playback_time >= (d_n - 1)))
              d_state = 2;
          }
        }
      }
      else if (d_state == 2) {
        consumed = 0;
        produced = -1;
      }

      if (d_state == 0 && d_input_done) {
        if (consumed == d_remaining_input)
          d_state = 1;
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace finite_stream */
} /* namespace gr */

