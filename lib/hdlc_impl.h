/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_AFSK_HDLC_IMPL_H
#define INCLUDED_AFSK_HDLC_IMPL_H

#include <afsk/hdlc.h>

#include <boost/crc.hpp>      // for boost::crc_basic, boost::augmented_crc
#include <boost/cstdint.hpp>  // for boost::uint16_t

namespace gr {
  namespace afsk {

    class hdlc_impl : public hdlc
    {
     private:
        // decoder states
        struct l1_state_afsk12 {
                  unsigned int dcd_shreg;
                  unsigned int sphase;
                  unsigned int lasts;
          } afsk12;
        // hdlc
        struct l2_state_hdlc {
                unsigned char rxbuf[512];
                unsigned char *rxptr;
                unsigned int rxstate;
                unsigned int rxbitstream;
                unsigned int rxbitbuf;
        } hdlc;

        // actual samplerate  of the decoder
        int d_sample_rate;
        int d_subsample;
        int d_sphaseinc;

        int d_numchars;

        char * out;

        int verbose_level;
        boost::crc_optimal<16,   0x1021, 0xFFFF, 0, true, true > crc_ccitt1;

        inline float mac(const float *a, const float *b, unsigned int size)
        {
                float sum = 0;
                unsigned int i;

                for (i = 0; i < size; i++)
                        sum += (*a++) * (*b++);
                return sum;
        }

        inline float fsqr(float f)
        {
                return f*f;
        }

        void hdlc_rxbit(int bit);
        void verbprintf(int verb_level, const char *fmt, ...);
        void ax25_disp_packet(unsigned char *bp, unsigned int len);
        void print_timestamp();

     public:
      hdlc_impl(int sample_rate, int subsample,int debug_level);
      ~hdlc_impl();

      // Where all the action really happens
      int general_work (int noutput_items,
                        gr_vector_int &ninput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items);
    };

  } // namespace afsk
} // namespace gr

#endif /* INCLUDED_AFSK_HDLC_IMPL_H */

