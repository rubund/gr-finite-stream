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


#ifndef INCLUDED_FINITE_STREAM_COUNT_ITEMS_H
#define INCLUDED_FINITE_STREAM_COUNT_ITEMS_H

#include <finite_stream/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace finite_stream {

    /*!
     * \brief <+description of block+>
     * \ingroup finite_stream
     *
     */
    class FINITE_STREAM_API count_items : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<count_items> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of finite_stream::count_items.
       *
       * To avoid accidental use of raw pointers, finite_stream::count_items's
       * constructor is in a private implementation
       * class. finite_stream::count_items::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize);
    };

  } // namespace finite_stream
} // namespace gr

#endif /* INCLUDED_FINITE_STREAM_COUNT_ITEMS_H */

