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

#ifndef INCLUDED_FINITE_STREAM_REPEAT_STREAM_IMPL_H
#define INCLUDED_FINITE_STREAM_REPEAT_STREAM_IMPL_H

#include <finite_stream/repeat_stream.h>
#include <gnuradio/block_detail.h>

namespace gr {
  namespace finite_stream {

    class repeat_stream_impl : public repeat_stream
    {
     private:
      int d_itemsize;
      int d_n;
      int d_max_samples;
      int d_state;
      int d_memory_cnt;
      int d_playback_cnt;
      int d_playback_time;
      bool d_input_done;

      int d_remaining_input;
      int d_max_n_memory_pages;

      int d_pagesize;

      int pagesize();

      gr::block_detail *d_block_detail;

      std::vector<char *> d_memory_pointers;

      void copy_to_memory(int pos, const char *src, int length);
      void copy_from_memory(char *dst, int pos, int length);

     public:
      repeat_stream_impl(size_t itemsize, int n, int max_samples);
      ~repeat_stream_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      bool start();
    };

  } // namespace finite_stream
} // namespace gr

#endif /* INCLUDED_FINITE_STREAM_REPEAT_STREAM_IMPL_H */

