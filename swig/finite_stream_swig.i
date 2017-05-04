/* -*- c++ -*- */

#define FINITE_STREAM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "finite_stream_swig_doc.i"

%{
#include "finite_stream/concatenate.h"
%}


%include "finite_stream/concatenate.h"
GR_SWIG_BLOCK_MAGIC2(finite_stream, concatenate);