
gr-finite-stream
================

Blocks for handling finite streams
----------------------------------

This out-of-tree module contains blocks used for handling finite length streams.
Currently, it only contains the concatenate block, but a repeat block is planned.


Description of the concatenate block
------------------------------------

This block takes the input from several sources and put all the items
after each other. All the items from the source connected to the
first input are produced first, then all the items from the second
source, and so on. The upstream blocks need to return -1 in the
general\_work method to make this block aware of when they have
completed producing items.

It is instantiated with:

```python
import finite_stream
concatenation = finite_stream.concatenate(gr.sizeof_gr_complex, 4)
```

This instance will then accept 4 complex streams. It is expected that all of them
are of finite length.
