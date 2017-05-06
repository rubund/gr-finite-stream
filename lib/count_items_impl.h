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

#ifndef INCLUDED_FINITE_STREAM_COUNT_ITEMS_IMPL_H
#define INCLUDED_FINITE_STREAM_COUNT_ITEMS_IMPL_H

#include <finite_stream/count_items.h>

namespace gr {
  namespace finite_stream {

    class count_items_impl : public count_items
    {
     private:
      // Nothing to declare in this block.
        unsigned long long d_count;
        size_t d_itemsize;
        bool d_done;
        int d_samples_left;
        bool d_ended;

     public:
      count_items_impl(size_t itemsize);
      ~count_items_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace finite_stream
} // namespace gr

#endif /* INCLUDED_FINITE_STREAM_COUNT_ITEMS_IMPL_H */

