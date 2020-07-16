/*
* -----------------------------------------------------------------
* COMPANY : Ruhr University Bochum
* AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
* DOCUMENT: https://eprint.iacr.org/2020/890.pdf
* -----------------------------------------------------------------
*
* Copyright (c) 2020, Amir Moradi, Aein Rezaei Shahmirzadi
*
* All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Please see LICENSE and README for license and further instructions.
*/

module MaskedSbox ( input1, input2, clk, output1, output2 );
  (* SILVER="[3:0]_0" *) input [3:0] input1;
  (* SILVER="[3:0]_1" *) input [3:0] input2;
  (* SILVER="[3:0]_0" *) output [3:0] output1;
  (* SILVER="[3:0]_1" *) output [3:0] output2;
  (* SILVER="clock" *) input clk;
  wire   Inst_N_F1_n11, Inst_N_F1_n10, Inst_N_F1_n9, Inst_N_F1_n8,
         Inst_N_F1_n7, Inst_N_F1_n6, Inst_N_F1_n5, Inst_N_F1_n4, Inst_N_F1_n3,
         Inst_N_F1_n2, Inst_N_F1_n1, Inst_N_F2_n9, Inst_N_F2_n8, Inst_N_F2_n7,
         Inst_N_F2_n6, Inst_N_F2_n5, Inst_N_F2_n4, Inst_N_F2_n3, Inst_N_F2_n2,
         Inst_N_F2_n1, Inst_N_F3_n10, Inst_N_F3_n9, Inst_N_F3_n8, Inst_N_F3_n7,
         Inst_N_F3_n6, Inst_N_F3_n5, Inst_N_F3_n4, Inst_N_F3_n3, Inst_N_F3_n2,
         Inst_N_F3_n1, Inst_N_F4_n17, Inst_N_F4_n16, Inst_N_F4_n15,
         Inst_N_F4_n14, Inst_N_F4_n13, Inst_N_F4_n12, Inst_N_F4_n11,
         Inst_N_F4_n10, Inst_N_F4_n9, Inst_N_F4_n8, Inst_N_F4_n7, Inst_N_F4_n6,
         Inst_N_F4_n5, Inst_N_F4_n4, Inst_N_F4_n3, Inst_N_F4_n2, Inst_N_F4_n1,
         Inst_N_F5_n13, Inst_N_F5_n12, Inst_N_F5_n11, Inst_N_F5_n10,
         Inst_N_F5_n9, Inst_N_F5_n8, Inst_N_F5_n7, Inst_N_F5_n6, Inst_N_F5_n5,
         Inst_N_F5_n4, Inst_N_F5_n3, Inst_N_F5_n2, Inst_N_F5_n1, Inst_N_F6_n14,
         Inst_N_F6_n13, Inst_N_F6_n12, Inst_N_F6_n11, Inst_N_F6_n10,
         Inst_N_F6_n9, Inst_N_F6_n8, Inst_N_F6_n7, Inst_N_F6_n6, Inst_N_F6_n5,
         Inst_N_F6_n4, Inst_N_F6_n3, Inst_N_F6_n2, Inst_N_F6_n1, Inst_N_F7_n7,
         Inst_N_F7_n6, Inst_N_F7_n5, Inst_N_F7_n4, Inst_N_F7_n3, Inst_N_F7_n2,
         Inst_N_F7_n1, Inst_N_F8_n7, Inst_N_F8_n6, Inst_N_F8_n5, Inst_N_F8_n4,
         Inst_N_F8_n3, Inst_N_F8_n2, Inst_N_F8_n1, Inst_L_XORInst1_n6,
         Inst_L_XORInst1_n5, Inst_L_XORInst2_n6, Inst_L_XORInst2_n5,
         Inst_L_XORInst5_n6, Inst_L_XORInst5_n5, Inst_L_XORInst6_n6,
         Inst_L_XORInst6_n5, Inst_L_XORInst7_n6, Inst_L_XORInst7_n5,
         Inst_L_XORInst8_n6, Inst_L_XORInst8_n5;
  wire   [4:1] e;
  wire   [8:1] f;
  wire   [8:1] g;
  wire   [8:1] h;
  wire   [8:1] f_reg;
  wire   [4:1] e_reg;
  wire   [8:1] g_reg;
  wire   [8:1] h_reg;

  INV_X1 Inst_N_F1_U15 ( .A(h[1]), .ZN(f[1]) );
  NAND2_X1 Inst_N_F1_U14 ( .A1(Inst_N_F1_n11), .A2(Inst_N_F1_n10), .ZN(g[1])
         );
  NOR2_X1 Inst_N_F1_U13 ( .A1(input1[3]), .A2(Inst_N_F1_n9), .ZN(Inst_N_F1_n10) );
  NOR2_X1 Inst_N_F1_U12 ( .A1(input1[2]), .A2(input1[1]), .ZN(Inst_N_F1_n9) );
  XNOR2_X1 Inst_N_F1_U11 ( .A(Inst_N_F1_n8), .B(Inst_N_F1_n7), .ZN(e[1]) );
  NAND2_X1 Inst_N_F1_U10 ( .A1(input1[2]), .A2(Inst_N_F1_n6), .ZN(Inst_N_F1_n7) );
  INV_X1 Inst_N_F1_U9 ( .A(input1[1]), .ZN(Inst_N_F1_n6) );
  XOR2_X1 Inst_N_F1_U8 ( .A(input2[0]), .B(input1[3]), .Z(Inst_N_F1_n8) );
  NAND2_X1 Inst_N_F1_U7 ( .A1(Inst_N_F1_n11), .A2(Inst_N_F1_n5), .ZN(h[1]) );
  NOR2_X1 Inst_N_F1_U6 ( .A1(Inst_N_F1_n4), .A2(Inst_N_F1_n3), .ZN(
        Inst_N_F1_n5) );
  NOR2_X1 Inst_N_F1_U5 ( .A1(input1[2]), .A2(Inst_N_F1_n2), .ZN(Inst_N_F1_n3)
         );
  INV_X1 Inst_N_F1_U4 ( .A(input1[3]), .ZN(Inst_N_F1_n2) );
  NOR2_X1 Inst_N_F1_U3 ( .A1(input1[3]), .A2(input1[1]), .ZN(Inst_N_F1_n4) );
  NAND2_X1 Inst_N_F1_U2 ( .A1(input1[1]), .A2(input1[2]), .ZN(Inst_N_F1_n1) );
  AND2_X1 Inst_N_F1_U1 ( .A1(Inst_N_F1_n1), .A2(input2[0]), .ZN(Inst_N_F1_n11)
         );
  NOR2_X1 Inst_N_F2_U12 ( .A1(input2[3]), .A2(Inst_N_F2_n9), .ZN(g[2]) );
  NAND2_X1 Inst_N_F2_U11 ( .A1(Inst_N_F2_n8), .A2(input1[0]), .ZN(Inst_N_F2_n9) );
  XNOR2_X1 Inst_N_F2_U10 ( .A(input1[2]), .B(input1[1]), .ZN(Inst_N_F2_n8) );
  NOR2_X1 Inst_N_F2_U9 ( .A1(Inst_N_F2_n7), .A2(Inst_N_F2_n6), .ZN(f[2]) );
  INV_X1 Inst_N_F2_U8 ( .A(input1[0]), .ZN(Inst_N_F2_n6) );
  NOR2_X1 Inst_N_F2_U7 ( .A1(input1[0]), .A2(Inst_N_F2_n7), .ZN(h[2]) );
  NOR2_X1 Inst_N_F2_U6 ( .A1(Inst_N_F2_n5), .A2(Inst_N_F2_n4), .ZN(
        Inst_N_F2_n7) );
  NOR2_X1 Inst_N_F2_U5 ( .A1(input2[3]), .A2(Inst_N_F2_n3), .ZN(Inst_N_F2_n4)
         );
  NAND2_X1 Inst_N_F2_U4 ( .A1(input1[1]), .A2(input1[2]), .ZN(Inst_N_F2_n3) );
  NOR2_X1 Inst_N_F2_U3 ( .A1(input1[2]), .A2(Inst_N_F2_n2), .ZN(Inst_N_F2_n5)
         );
  NAND2_X1 Inst_N_F2_U2 ( .A1(input2[3]), .A2(Inst_N_F2_n1), .ZN(Inst_N_F2_n2)
         );
  INV_X1 Inst_N_F2_U1 ( .A(input1[1]), .ZN(Inst_N_F2_n1) );
  NOR2_X1 Inst_N_F3_U13 ( .A1(input1[0]), .A2(Inst_N_F3_n10), .ZN(g[3]) );
  NAND2_X1 Inst_N_F3_U12 ( .A1(input1[3]), .A2(Inst_N_F3_n9), .ZN(
        Inst_N_F3_n10) );
  NAND2_X1 Inst_N_F3_U11 ( .A1(Inst_N_F3_n8), .A2(Inst_N_F3_n7), .ZN(
        Inst_N_F3_n9) );
  NOR2_X1 Inst_N_F3_U10 ( .A1(input1[0]), .A2(Inst_N_F3_n6), .ZN(f[3]) );
  NOR2_X1 Inst_N_F3_U9 ( .A1(Inst_N_F3_n6), .A2(Inst_N_F3_n5), .ZN(h[3]) );
  INV_X1 Inst_N_F3_U8 ( .A(input1[0]), .ZN(Inst_N_F3_n5) );
  NOR2_X1 Inst_N_F3_U7 ( .A1(Inst_N_F3_n4), .A2(Inst_N_F3_n3), .ZN(
        Inst_N_F3_n6) );
  NOR2_X1 Inst_N_F3_U6 ( .A1(input1[3]), .A2(Inst_N_F3_n7), .ZN(Inst_N_F3_n3)
         );
  NAND2_X1 Inst_N_F3_U5 ( .A1(input2[2]), .A2(Inst_N_F3_n2), .ZN(Inst_N_F3_n7)
         );
  NOR2_X1 Inst_N_F3_U4 ( .A1(Inst_N_F3_n8), .A2(Inst_N_F3_n1), .ZN(
        Inst_N_F3_n4) );
  INV_X1 Inst_N_F3_U3 ( .A(input1[3]), .ZN(Inst_N_F3_n1) );
  OR2_X1 Inst_N_F3_U2 ( .A1(Inst_N_F3_n2), .A2(input2[2]), .ZN(Inst_N_F3_n8)
         );
  INV_X1 Inst_N_F3_U1 ( .A(input1[1]), .ZN(Inst_N_F3_n2) );
  NAND2_X1 Inst_N_F4_U21 ( .A1(Inst_N_F4_n17), .A2(Inst_N_F4_n16), .ZN(g[4])
         );
  NAND2_X1 Inst_N_F4_U20 ( .A1(Inst_N_F4_n15), .A2(Inst_N_F4_n14), .ZN(
        Inst_N_F4_n16) );
  XNOR2_X1 Inst_N_F4_U19 ( .A(input2[2]), .B(Inst_N_F4_n13), .ZN(Inst_N_F4_n14) );
  NAND2_X1 Inst_N_F4_U18 ( .A1(input2[3]), .A2(Inst_N_F4_n12), .ZN(
        Inst_N_F4_n17) );
  NOR2_X1 Inst_N_F4_U17 ( .A1(Inst_N_F4_n11), .A2(Inst_N_F4_n10), .ZN(
        Inst_N_F4_n12) );
  NOR2_X1 Inst_N_F4_U16 ( .A1(input2[0]), .A2(input1[1]), .ZN(Inst_N_F4_n10)
         );
  NOR2_X1 Inst_N_F4_U15 ( .A1(input2[2]), .A2(Inst_N_F4_n13), .ZN(
        Inst_N_F4_n11) );
  NAND2_X1 Inst_N_F4_U14 ( .A1(Inst_N_F4_n9), .A2(Inst_N_F4_n8), .ZN(f[4]) );
  NAND2_X1 Inst_N_F4_U13 ( .A1(Inst_N_F4_n7), .A2(Inst_N_F4_n6), .ZN(
        Inst_N_F4_n9) );
  XNOR2_X1 Inst_N_F4_U12 ( .A(Inst_N_F4_n13), .B(Inst_N_F4_n15), .ZN(
        Inst_N_F4_n7) );
  XNOR2_X1 Inst_N_F4_U11 ( .A(Inst_N_F4_n5), .B(Inst_N_F4_n13), .ZN(e[2]) );
  NOR2_X1 Inst_N_F4_U10 ( .A1(Inst_N_F4_n4), .A2(input1[1]), .ZN(Inst_N_F4_n5)
         );
  INV_X1 Inst_N_F4_U9 ( .A(input2[2]), .ZN(Inst_N_F4_n4) );
  NAND2_X1 Inst_N_F4_U8 ( .A1(Inst_N_F4_n3), .A2(Inst_N_F4_n8), .ZN(h[4]) );
  NAND2_X1 Inst_N_F4_U7 ( .A1(Inst_N_F4_n2), .A2(Inst_N_F4_n13), .ZN(
        Inst_N_F4_n8) );
  AND2_X1 Inst_N_F4_U6 ( .A1(input2[2]), .A2(input1[1]), .ZN(Inst_N_F4_n2) );
  NAND2_X1 Inst_N_F4_U5 ( .A1(Inst_N_F4_n6), .A2(Inst_N_F4_n15), .ZN(
        Inst_N_F4_n3) );
  INV_X1 Inst_N_F4_U4 ( .A(input2[3]), .ZN(Inst_N_F4_n15) );
  NAND2_X1 Inst_N_F4_U3 ( .A1(Inst_N_F4_n13), .A2(Inst_N_F4_n1), .ZN(
        Inst_N_F4_n6) );
  NOR2_X1 Inst_N_F4_U2 ( .A1(input2[2]), .A2(input1[1]), .ZN(Inst_N_F4_n1) );
  INV_X1 Inst_N_F4_U1 ( .A(input2[0]), .ZN(Inst_N_F4_n13) );
  NAND2_X1 Inst_N_F5_U16 ( .A1(Inst_N_F5_n12), .A2(Inst_N_F5_n11), .ZN(f[5])
         );
  NAND2_X1 Inst_N_F5_U15 ( .A1(input1[0]), .A2(Inst_N_F5_n13), .ZN(
        Inst_N_F5_n11) );
  NAND2_X1 Inst_N_F5_U14 ( .A1(Inst_N_F5_n10), .A2(Inst_N_F5_n9), .ZN(
        Inst_N_F5_n12) );
  NOR2_X1 Inst_N_F5_U13 ( .A1(input1[0]), .A2(Inst_N_F5_n8), .ZN(Inst_N_F5_n10) );
  NOR2_X1 Inst_N_F5_U12 ( .A1(input2[1]), .A2(Inst_N_F5_n7), .ZN(Inst_N_F5_n8)
         );
  NOR2_X1 Inst_N_F5_U11 ( .A1(Inst_N_F5_n13), .A2(input1[2]), .ZN(Inst_N_F5_n7) );
  AND2_X1 Inst_N_F5_U10 ( .A1(Inst_N_F5_n9), .A2(Inst_N_F5_n6), .ZN(h[5]) );
  NAND2_X1 Inst_N_F5_U9 ( .A1(Inst_N_F5_n5), .A2(Inst_N_F5_n4), .ZN(
        Inst_N_F5_n6) );
  NAND2_X1 Inst_N_F5_U8 ( .A1(input2[1]), .A2(input1[0]), .ZN(Inst_N_F5_n4) );
  NAND2_X1 Inst_N_F5_U7 ( .A1(Inst_N_F5_n3), .A2(input1[3]), .ZN(Inst_N_F5_n5)
         );
  NAND2_X1 Inst_N_F5_U6 ( .A1(input1[2]), .A2(input1[0]), .ZN(Inst_N_F5_n3) );
  NAND2_X1 Inst_N_F5_U5 ( .A1(input1[2]), .A2(Inst_N_F5_n13), .ZN(Inst_N_F5_n9) );
  INV_X1 Inst_N_F5_U4 ( .A(input1[3]), .ZN(Inst_N_F5_n13) );
  NOR2_X1 Inst_N_F5_U3 ( .A1(Inst_N_F5_n1), .A2(Inst_N_F5_n2), .ZN(g[5]) );
  NAND2_X1 Inst_N_F5_U2 ( .A1(input1[0]), .A2(Inst_N_F5_n13), .ZN(Inst_N_F5_n2) );
  XOR2_X1 Inst_N_F5_U1 ( .A(input1[2]), .B(input2[1]), .Z(Inst_N_F5_n1) );
  NAND2_X1 Inst_N_F6_U17 ( .A1(Inst_N_F6_n14), .A2(Inst_N_F6_n13), .ZN(g[6])
         );
  NAND2_X1 Inst_N_F6_U16 ( .A1(Inst_N_F6_n12), .A2(Inst_N_F6_n11), .ZN(
        Inst_N_F6_n13) );
  NOR2_X1 Inst_N_F6_U15 ( .A1(Inst_N_F6_n10), .A2(Inst_N_F6_n9), .ZN(
        Inst_N_F6_n12) );
  NAND2_X1 Inst_N_F6_U14 ( .A1(Inst_N_F6_n8), .A2(Inst_N_F6_n9), .ZN(
        Inst_N_F6_n14) );
  NAND2_X1 Inst_N_F6_U13 ( .A1(Inst_N_F6_n7), .A2(Inst_N_F6_n11), .ZN(
        Inst_N_F6_n8) );
  OR2_X1 Inst_N_F6_U12 ( .A1(Inst_N_F6_n6), .A2(input2[3]), .ZN(Inst_N_F6_n7)
         );
  NOR2_X1 Inst_N_F6_U11 ( .A1(input2[0]), .A2(Inst_N_F6_n5), .ZN(f[6]) );
  NOR2_X1 Inst_N_F6_U10 ( .A1(Inst_N_F6_n4), .A2(Inst_N_F6_n3), .ZN(
        Inst_N_F6_n5) );
  NOR2_X1 Inst_N_F6_U9 ( .A1(input2[1]), .A2(Inst_N_F6_n11), .ZN(Inst_N_F6_n3)
         );
  NAND2_X1 Inst_N_F6_U8 ( .A1(input2[3]), .A2(Inst_N_F6_n6), .ZN(Inst_N_F6_n11) );
  INV_X1 Inst_N_F6_U7 ( .A(input1[2]), .ZN(Inst_N_F6_n6) );
  AND2_X1 Inst_N_F6_U6 ( .A1(input1[2]), .A2(Inst_N_F6_n10), .ZN(Inst_N_F6_n4)
         );
  NOR2_X1 Inst_N_F6_U5 ( .A1(input2[3]), .A2(Inst_N_F6_n2), .ZN(Inst_N_F6_n10)
         );
  INV_X1 Inst_N_F6_U4 ( .A(input2[1]), .ZN(Inst_N_F6_n2) );
  XOR2_X1 Inst_N_F6_U3 ( .A(Inst_N_F6_n1), .B(Inst_N_F6_n9), .Z(e[3]) );
  NAND2_X1 Inst_N_F6_U2 ( .A1(input2[1]), .A2(input1[2]), .ZN(Inst_N_F6_n1) );
  INV_X1 Inst_N_F6_U1 ( .A(input2[0]), .ZN(Inst_N_F6_n9) );
  NOR2_X1 Inst_N_F7_U9 ( .A1(Inst_N_F7_n7), .A2(Inst_N_F7_n6), .ZN(g[7]) );
  OR2_X1 Inst_N_F7_U8 ( .A1(input2[0]), .A2(Inst_N_F7_n5), .ZN(Inst_N_F7_n6)
         );
  NOR2_X1 Inst_N_F7_U7 ( .A1(Inst_N_F7_n4), .A2(Inst_N_F7_n3), .ZN(f[7]) );
  NOR2_X1 Inst_N_F7_U6 ( .A1(input1[3]), .A2(Inst_N_F7_n5), .ZN(Inst_N_F7_n3)
         );
  NOR2_X1 Inst_N_F7_U5 ( .A1(input2[1]), .A2(Inst_N_F7_n2), .ZN(Inst_N_F7_n5)
         );
  NAND2_X1 Inst_N_F7_U4 ( .A1(Inst_N_F7_n7), .A2(input2[0]), .ZN(Inst_N_F7_n4)
         );
  NAND2_X1 Inst_N_F7_U3 ( .A1(input1[3]), .A2(Inst_N_F7_n1), .ZN(Inst_N_F7_n7)
         );
  NAND2_X1 Inst_N_F7_U2 ( .A1(input2[1]), .A2(Inst_N_F7_n2), .ZN(Inst_N_F7_n1)
         );
  INV_X1 Inst_N_F7_U1 ( .A(input2[2]), .ZN(Inst_N_F7_n2) );
  NOR2_X1 Inst_N_F8_U11 ( .A1(Inst_N_F8_n7), .A2(Inst_N_F8_n6), .ZN(g[8]) );
  NAND2_X1 Inst_N_F8_U10 ( .A1(input2[3]), .A2(Inst_N_F8_n5), .ZN(Inst_N_F8_n6) );
  INV_X1 Inst_N_F8_U9 ( .A(input1[0]), .ZN(Inst_N_F8_n5) );
  NOR2_X1 Inst_N_F8_U8 ( .A1(Inst_N_F8_n4), .A2(Inst_N_F8_n3), .ZN(f[8]) );
  NAND2_X1 Inst_N_F8_U7 ( .A1(input1[0]), .A2(Inst_N_F8_n7), .ZN(Inst_N_F8_n3)
         );
  XOR2_X1 Inst_N_F8_U6 ( .A(input1[0]), .B(Inst_N_F8_n4), .Z(e[4]) );
  NAND2_X1 Inst_N_F8_U5 ( .A1(Inst_N_F8_n2), .A2(Inst_N_F8_n7), .ZN(h[8]) );
  XNOR2_X1 Inst_N_F8_U4 ( .A(input2[1]), .B(input2[2]), .ZN(Inst_N_F8_n7) );
  NOR2_X1 Inst_N_F8_U3 ( .A1(input1[0]), .A2(Inst_N_F8_n4), .ZN(Inst_N_F8_n2)
         );
  XNOR2_X1 Inst_N_F8_U2 ( .A(input2[3]), .B(Inst_N_F8_n1), .ZN(Inst_N_F8_n4)
         );
  NAND2_X1 Inst_N_F8_U1 ( .A1(input2[1]), .A2(input2[2]), .ZN(Inst_N_F8_n1) );
  DFF_X1 Gen_reg_Q_reg_0_ ( .D(f[1]), .CK(clk), .Q(f_reg[1]), .QN() );
  DFF_X1 Gen_reg_Q_reg_1_ ( .D(f[2]), .CK(clk), .Q(f_reg[2]), .QN() );
  DFF_X1 Gen_reg_Q_reg_2_ ( .D(f[3]), .CK(clk), .Q(f_reg[3]), .QN() );
  DFF_X1 Gen_reg_Q_reg_3_ ( .D(f[4]), .CK(clk), .Q(f_reg[4]), .QN() );
  DFF_X1 Gen_reg_Q_reg_4_ ( .D(f[5]), .CK(clk), .Q(f_reg[5]), .QN() );
  DFF_X1 Gen_reg_Q_reg_5_ ( .D(f[6]), .CK(clk), .Q(f_reg[6]), .QN() );
  DFF_X1 Gen_reg_Q_reg_6_ ( .D(f[7]), .CK(clk), .Q(f_reg[7]), .QN() );
  DFF_X1 Gen_reg_Q_reg_7_ ( .D(f[8]), .CK(clk), .Q(f_reg[8]), .QN() );
  DFF_X1 Gen_reg_Q_reg_8_ ( .D(e[1]), .CK(clk), .Q(e_reg[1]), .QN() );
  DFF_X1 Gen_reg_Q_reg_9_ ( .D(e[2]), .CK(clk), .Q(e_reg[2]), .QN() );
  DFF_X1 Gen_reg_Q_reg_10_ ( .D(e[3]), .CK(clk), .Q(e_reg[3]), .QN() );
  DFF_X1 Gen_reg_Q_reg_11_ ( .D(e[4]), .CK(clk), .Q(e_reg[4]), .QN() );
  DFF_X1 Gen_reg_Q_reg_12_ ( .D(g[1]), .CK(clk), .Q(g_reg[1]), .QN() );
  DFF_X1 Gen_reg_Q_reg_13_ ( .D(g[2]), .CK(clk), .Q(g_reg[2]), .QN() );
  DFF_X1 Gen_reg_Q_reg_14_ ( .D(g[3]), .CK(clk), .Q(g_reg[3]), .QN() );
  DFF_X1 Gen_reg_Q_reg_15_ ( .D(g[4]), .CK(clk), .Q(g_reg[4]), .QN() );
  DFF_X1 Gen_reg_Q_reg_16_ ( .D(g[5]), .CK(clk), .Q(g_reg[5]), .QN() );
  DFF_X1 Gen_reg_Q_reg_17_ ( .D(g[6]), .CK(clk), .Q(g_reg[6]), .QN() );
  DFF_X1 Gen_reg_Q_reg_18_ ( .D(g[7]), .CK(clk), .Q(g_reg[7]), .QN() );
  DFF_X1 Gen_reg_Q_reg_19_ ( .D(g[8]), .CK(clk), .Q(g_reg[8]), .QN() );
  DFF_X1 Gen_reg_Q_reg_20_ ( .D(h[1]), .CK(clk), .Q(h_reg[1]), .QN() );
  DFF_X1 Gen_reg_Q_reg_21_ ( .D(h[2]), .CK(clk), .Q(h_reg[2]), .QN() );
  DFF_X1 Gen_reg_Q_reg_22_ ( .D(h[3]), .CK(clk), .Q(h_reg[3]), .QN() );
  DFF_X1 Gen_reg_Q_reg_23_ ( .D(h[4]), .CK(clk), .Q(h_reg[4]), .QN() );
  DFF_X1 Gen_reg_Q_reg_24_ ( .D(h[5]), .CK(clk), .Q(h_reg[5]), .QN() );
  DFF_X1 Gen_reg_Q_reg_25_ ( .D(f[6]), .CK(clk), .Q(h_reg[6]), .QN() );
  DFF_X1 Gen_reg_Q_reg_26_ ( .D(f[7]), .CK(clk), .Q(h_reg[7]), .QN() );
  DFF_X1 Gen_reg_Q_reg_27_ ( .D(h[8]), .CK(clk), .Q(h_reg[8]), .QN() );
  XNOR2_X1 Inst_L_XORInst1_U3 ( .A(Inst_L_XORInst1_n6), .B(Inst_L_XORInst1_n5), 
        .ZN(output1[1]) );
  XNOR2_X1 Inst_L_XORInst1_U2 ( .A(f_reg[3]), .B(f_reg[4]), .ZN(
        Inst_L_XORInst1_n5) );
  XOR2_X1 Inst_L_XORInst1_U1 ( .A(f_reg[1]), .B(f_reg[2]), .Z(
        Inst_L_XORInst1_n6) );
  XNOR2_X1 Inst_L_XORInst2_U3 ( .A(Inst_L_XORInst2_n6), .B(Inst_L_XORInst2_n5), 
        .ZN(output2[1]) );
  XNOR2_X1 Inst_L_XORInst2_U2 ( .A(f_reg[7]), .B(f_reg[8]), .ZN(
        Inst_L_XORInst2_n5) );
  XOR2_X1 Inst_L_XORInst2_U1 ( .A(f_reg[5]), .B(f_reg[6]), .Z(
        Inst_L_XORInst2_n6) );
  XOR2_X1 Inst_L_XORInst3_U1 ( .A(e_reg[1]), .B(e_reg[2]), .Z(output1[0]) );
  XOR2_X1 Inst_L_XORInst4_U1 ( .A(e_reg[3]), .B(e_reg[4]), .Z(output2[0]) );
  XNOR2_X1 Inst_L_XORInst5_U3 ( .A(Inst_L_XORInst5_n6), .B(Inst_L_XORInst5_n5), 
        .ZN(output1[2]) );
  XNOR2_X1 Inst_L_XORInst5_U2 ( .A(g_reg[3]), .B(g_reg[4]), .ZN(
        Inst_L_XORInst5_n5) );
  XOR2_X1 Inst_L_XORInst5_U1 ( .A(g_reg[1]), .B(g_reg[2]), .Z(
        Inst_L_XORInst5_n6) );
  XNOR2_X1 Inst_L_XORInst6_U3 ( .A(Inst_L_XORInst6_n6), .B(Inst_L_XORInst6_n5), 
        .ZN(output2[2]) );
  XNOR2_X1 Inst_L_XORInst6_U2 ( .A(g_reg[7]), .B(g_reg[8]), .ZN(
        Inst_L_XORInst6_n5) );
  XOR2_X1 Inst_L_XORInst6_U1 ( .A(g_reg[5]), .B(g_reg[6]), .Z(
        Inst_L_XORInst6_n6) );
  XNOR2_X1 Inst_L_XORInst7_U3 ( .A(Inst_L_XORInst7_n6), .B(Inst_L_XORInst7_n5), 
        .ZN(output1[3]) );
  XNOR2_X1 Inst_L_XORInst7_U2 ( .A(h_reg[3]), .B(h_reg[4]), .ZN(
        Inst_L_XORInst7_n5) );
  XOR2_X1 Inst_L_XORInst7_U1 ( .A(h_reg[1]), .B(h_reg[2]), .Z(
        Inst_L_XORInst7_n6) );
  XNOR2_X1 Inst_L_XORInst8_U3 ( .A(Inst_L_XORInst8_n6), .B(Inst_L_XORInst8_n5), 
        .ZN(output2[3]) );
  XNOR2_X1 Inst_L_XORInst8_U2 ( .A(h_reg[7]), .B(h_reg[8]), .ZN(
        Inst_L_XORInst8_n5) );
  XOR2_X1 Inst_L_XORInst8_U1 ( .A(h_reg[5]), .B(h_reg[6]), .Z(
        Inst_L_XORInst8_n6) );
endmodule

