#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import finite_stream_swig as finite_stream

class qa_repeat_stream (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src = blocks.vector_source_c([1,2,3,4])
        dut = finite_stream.repeat_stream(gr.sizeof_gr_complex, 3)
        dst = blocks.vector_sink_c()
        self.tb.connect(src,dut,dst)
        self.tb.run ()
        # check data
        self.assertEqual(dst.data() , ((1+0j), (2+0j), (3+0j), (4+0j), (1+0j), (2+0j), (3+0j), (4+0j), (1+0j), (2+0j), (3+0j), (4+0j)))

    def test_002_t (self):
        # set up fg
        src = blocks.vector_source_c([2]*1000000)
        dut = finite_stream.repeat_stream(gr.sizeof_gr_complex, 3)
        dst = blocks.vector_sink_c()
        self.tb.connect(src,dut,dst)
        self.tb.run ()
        # check data
        self.assertEqual(len(dst.data()) , 3000000)

    def test_003_t (self):
        # set up fg
        src = blocks.vector_source_c([2]*1000000)
        dut = finite_stream.repeat_stream(gr.sizeof_gr_complex, 3, 5)
        dst = blocks.vector_sink_c()
        self.tb.connect(src,dut,dst)
        self.tb.run ()
        # check data
        self.assertEqual(len(dst.data()) , 15)

if __name__ == '__main__':
    gr_unittest.run(qa_repeat_stream, "qa_repeat_stream.xml")
