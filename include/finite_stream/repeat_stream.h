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


#ifndef INCLUDED_FINITE_STREAM_REPEAT_STREAM_H
#define INCLUDED_FINITE_STREAM_REPEAT_STREAM_H

#include <finite_stream/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace finite_stream {

    /*!
     * \brief <+description of block+>
     * \ingroup finite_stream
     *
     */
    class FINITE_STREAM_API repeat_stream : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<repeat_stream> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of grnewones::repeat_stream.
       *
       * To avoid accidental use of raw pointers, grnewones::repeat_stream's
       * constructor is in a private implementation
       * class. grnewones::repeat_stream::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, int n=-1, int max_samples=10000000);
    };

  } // namespace finite_stream
} // namespace gr

#endif /* INCLUDED_FINITE_STREAM_REPEAT_STREAM_H */

