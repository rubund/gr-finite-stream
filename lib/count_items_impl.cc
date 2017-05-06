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
#include <gnuradio/block_detail.h>
#include <gnuradio/buffer.h>
#include <boost/thread.hpp>
#include "count_items_impl.h"

namespace gr {
  namespace finite_stream {

    count_items::sptr
    count_items::make(size_t itemsize)
    {
      return gnuradio::get_initial_sptr
        (new count_items_impl(itemsize));
    }

    /*
     * The private constructor
     */
    count_items_impl::count_items_impl(size_t itemsize)
      : gr::block("count_items",
              gr::io_signature::make(1, 1, sizeof(itemsize)),
              gr::io_signature::make(1, 1, sizeof(int))),
            d_count(0), d_itemsize(itemsize), d_done(false),
            d_samples_left(0), d_ended(false)
    {}

    /*
     * Our virtual destructor.
     */
    count_items_impl::~count_items_impl()
    {
    }

    void
    count_items_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      int input_done;
      int input_items_available;
      int current_ninput_items_required;

      current_ninput_items_required = noutput_items;
      {
        gr::block_detail *d_block_detail;
        d_block_detail = this->detail().get();
        gr::thread::scoped_lock guard(*d_block_detail->input(0)->mutex());
        input_items_available = d_block_detail->input(0)->items_available ();
        input_done = d_block_detail->input(0)->done();
        if(input_done) {
          d_done = true;
          d_samples_left = input_items_available;
          current_ninput_items_required = 0;
        }
      }
      if (!d_done) {
          ninput_items_required[0] = current_ninput_items_required;
      }
      else {
          ninput_items_required[0] = 0;
      }

    }

    int
    count_items_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      int *out = (int *) output_items[0];
      int consumed = 0;



      if(d_ended)
        return -1;

      int count = std::min(noutput_items, ninput_items[0]);

      d_count += count;

      consumed = count;
      consume_each (consumed);

      if (d_done && consumed == d_samples_left) {
          printf("Count: %d\n", d_count);
          d_ended = true;
      }

      return consumed;
    }

  } /* namespace finite_stream */
} /* namespace gr */

