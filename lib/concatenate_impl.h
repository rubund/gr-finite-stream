/* -*- c++ -*- */
/* 
 * Copyright 2017 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_FINITE_STREAM_CONCATENATE_IMPL_H
#define INCLUDED_FINITE_STREAM_CONCATENATE_IMPL_H

#include <finite_stream/concatenate.h>
#include <gnuradio/block_detail.h>

static const pmt::pmt_t BEGIN_KEY = pmt::string_to_symbol("concatenated");

namespace gr {
  namespace finite_stream {

    class concatenate_impl : public concatenate
    {
     private:
      size_t d_itemsize;
      int d_ninputs;
      bool d_current_done;
      int d_current_input;
      bool d_tag_parts;
      int d_last_input;

      int d_current_samples_left;
      pmt::pmt_t _id;

      gr::block_detail *d_block_detail;

     public:
      concatenate_impl(size_t itemsize, int ninputs);
      ~concatenate_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      bool start();

      void set_tag_parts(bool);
    };

  } // namespace finite_stream
} // namespace gr

#endif /* INCLUDED_FINITE_STREAM_CONCATENATE_IMPL_H */

