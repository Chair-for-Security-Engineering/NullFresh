/*
* -----------------------------------------------------------------
* COMPANY : Ruhr University Bochum
* AUTHOR  : Amir Moradi (amir.moradi@rub.de) Aein Rezaei Shahmirzadi (aein.rezaeishahmirzadi@rub.de)
* DOCUMENT: https://eprint.iacr.org/2020/
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
  wire   Inst_N_F1_n19, Inst_N_F1_n18, Inst_N_F1_n17, Inst_N_F1_n16,
         Inst_N_F1_n15, Inst_N_F1_n14, Inst_N_F1_n13, Inst_N_F1_n12,
         Inst_N_F1_n11, Inst_N_F1_n10, Inst_N_F1_n9, Inst_N_F1_n8,
         Inst_N_F1_n7, Inst_N_F1_n6, Inst_N_F1_n5, Inst_N_F1_n4, Inst_N_F2_n16,
         Inst_N_F2_n15, Inst_N_F2_n14, Inst_N_F2_n13, Inst_N_F2_n12,
         Inst_N_F2_n11, Inst_N_F2_n10, Inst_N_F2_n9, Inst_N_F2_n8,
         Inst_N_F2_n7, Inst_N_F2_n6, Inst_N_F2_n5, Inst_N_F2_n4, Inst_N_F2_n3,
         Inst_N_F3_n57, Inst_N_F3_n56, Inst_N_F3_n55, Inst_N_F3_n54,
         Inst_N_F3_n53, Inst_N_F3_n52, Inst_N_F3_n51, Inst_N_F3_n50,
         Inst_N_F3_n49, Inst_N_F3_n48, Inst_N_F3_n47, Inst_N_F3_n46,
         Inst_N_F3_n45, Inst_N_F3_n44, Inst_N_F3_n43, Inst_N_F3_n42,
         Inst_N_F4_n27, Inst_N_F4_n26, Inst_N_F4_n25, Inst_N_F4_n24,
         Inst_N_F4_n23, Inst_N_F4_n22, Inst_N_F4_n21, Inst_N_F4_n20,
         Inst_N_F4_n19, Inst_N_F4_n18, Inst_N_F4_n17, Inst_N_F4_n16,
         Inst_N_F4_n15, Inst_N_F4_n14, Inst_N_F4_n13, Inst_N_F4_n12,
         Inst_N_F4_n11, Inst_N_F4_n10, Inst_N_F4_n9, Inst_N_F5_n25,
         Inst_N_F5_n24, Inst_N_F5_n23, Inst_N_F5_n22, Inst_N_F5_n21,
         Inst_N_F5_n20, Inst_N_F5_n19, Inst_N_F5_n18, Inst_N_F5_n17,
         Inst_N_F5_n16, Inst_N_F5_n15, Inst_N_F5_n14, Inst_N_F5_n13,
         Inst_N_F5_n12, Inst_N_F5_n11, Inst_N_F5_n10, Inst_N_F5_n9,
         Inst_N_F5_n8, Inst_N_F5_n7, Inst_N_F5_n6, Inst_N_F5_n5, Inst_N_F5_n4,
         Inst_N_F6_n28, Inst_N_F6_n27, Inst_N_F6_n26, Inst_N_F6_n25,
         Inst_N_F6_n24, Inst_N_F6_n23, Inst_N_F6_n22, Inst_N_F6_n21,
         Inst_N_F6_n20, Inst_N_F6_n19, Inst_N_F6_n18, Inst_N_F6_n17,
         Inst_N_F6_n16, Inst_N_F6_n15, Inst_N_F6_n14, Inst_N_F6_n13,
         Inst_N_F6_n12, Inst_N_F6_n11, Inst_N_F6_n10, Inst_N_F6_n9,
         Inst_N_F6_n8, Inst_N_F7_n27, Inst_N_F7_n26, Inst_N_F7_n25,
         Inst_N_F7_n24, Inst_N_F7_n23, Inst_N_F7_n22, Inst_N_F7_n21,
         Inst_N_F7_n20, Inst_N_F7_n19, Inst_N_F7_n18, Inst_N_F7_n17,
         Inst_N_F7_n16, Inst_N_F7_n15, Inst_N_F7_n14, Inst_N_F7_n13,
         Inst_N_F7_n12, Inst_N_F7_n11, Inst_N_F7_n10, Inst_N_F7_n9,
         Inst_N_F7_n8, Inst_N_F8_n18, Inst_N_F8_n17, Inst_N_F8_n16,
         Inst_N_F8_n15, Inst_N_F8_n14, Inst_N_F8_n13, Inst_N_F8_n12,
         Inst_N_F8_n11, Inst_N_F8_n10, Inst_N_F8_n9, Inst_N_F8_n8,
         Inst_N_F8_n7, Inst_N_F8_n6, Inst_N_F8_n5, Inst_N_F8_n4, Inst_N_F8_n3,
         Inst_N_F8_n2, Inst_N_F8_n1, Inst_L_XORInst1_n6, Inst_L_XORInst1_n5,
         Inst_L_XORInst2_n6, Inst_L_XORInst2_n5, Inst_L_XORInst3_n6,
         Inst_L_XORInst3_n5, Inst_L_XORInst4_n6, Inst_L_XORInst4_n5,
         Inst_L_XORInst5_n6, Inst_L_XORInst5_n5, Inst_L_XORInst6_n6,
         Inst_L_XORInst6_n5, Inst_L_XORInst7_n6, Inst_L_XORInst7_n5,
         Inst_L_XORInst8_n6, Inst_L_XORInst8_n5;
  wire   [8:1] e;
  wire   [8:1] f;
  wire   [8:1] g;
  wire   [8:1] h;
  wire   [8:1] e_reg;
  wire   [8:1] f_reg;
  wire   [8:1] g_reg;
  wire   [8:1] h_reg;

  NAND2_X1 Inst_N_F1_U20 ( .A1(Inst_N_F1_n19), .A2(Inst_N_F1_n18), .ZN(h[1])
         );
  NOR2_X1 Inst_N_F1_U19 ( .A1(input1[2]), .A2(Inst_N_F1_n17), .ZN(
        Inst_N_F1_n18) );
  NOR2_X1 Inst_N_F1_U18 ( .A1(input1[0]), .A2(Inst_N_F1_n16), .ZN(
        Inst_N_F1_n17) );
  NAND2_X1 Inst_N_F1_U17 ( .A1(Inst_N_F1_n15), .A2(Inst_N_F1_n14), .ZN(
        Inst_N_F1_n19) );
  NAND2_X1 Inst_N_F1_U16 ( .A1(Inst_N_F1_n13), .A2(input1[1]), .ZN(
        Inst_N_F1_n15) );
  NAND2_X1 Inst_N_F1_U15 ( .A1(Inst_N_F1_n12), .A2(Inst_N_F1_n11), .ZN(g[1])
         );
  NAND2_X1 Inst_N_F1_U14 ( .A1(Inst_N_F1_n10), .A2(input1[0]), .ZN(
        Inst_N_F1_n11) );
  NOR2_X1 Inst_N_F1_U13 ( .A1(input1[1]), .A2(Inst_N_F1_n9), .ZN(Inst_N_F1_n10) );
  NAND2_X1 Inst_N_F1_U12 ( .A1(Inst_N_F1_n8), .A2(input1[2]), .ZN(
        Inst_N_F1_n12) );
  NAND2_X1 Inst_N_F1_U11 ( .A1(Inst_N_F1_n14), .A2(input1[0]), .ZN(
        Inst_N_F1_n8) );
  NAND2_X1 Inst_N_F1_U10 ( .A1(Inst_N_F1_n7), .A2(e[1]), .ZN(f[1]) );
  NAND2_X1 Inst_N_F1_U9 ( .A1(Inst_N_F1_n6), .A2(Inst_N_F1_n13), .ZN(
        Inst_N_F1_n7) );
  NAND2_X1 Inst_N_F1_U8 ( .A1(Inst_N_F1_n5), .A2(input1[2]), .ZN(Inst_N_F1_n6)
         );
  NAND2_X1 Inst_N_F1_U7 ( .A1(input1[1]), .A2(Inst_N_F1_n16), .ZN(Inst_N_F1_n5) );
  NAND2_X1 Inst_N_F1_U6 ( .A1(Inst_N_F1_n9), .A2(Inst_N_F1_n4), .ZN(e[1]) );
  NAND2_X1 Inst_N_F1_U5 ( .A1(Inst_N_F1_n13), .A2(Inst_N_F1_n14), .ZN(
        Inst_N_F1_n4) );
  OR2_X1 Inst_N_F1_U4 ( .A1(Inst_N_F1_n16), .A2(input1[1]), .ZN(Inst_N_F1_n14)
         );
  INV_X1 Inst_N_F1_U3 ( .A(input2[3]), .ZN(Inst_N_F1_n16) );
  INV_X1 Inst_N_F1_U2 ( .A(input1[0]), .ZN(Inst_N_F1_n13) );
  OR2_X1 Inst_N_F1_U1 ( .A1(input1[2]), .A2(input2[3]), .ZN(Inst_N_F1_n9) );
  NAND2_X1 Inst_N_F2_U18 ( .A1(Inst_N_F2_n15), .A2(Inst_N_F2_n14), .ZN(e[2])
         );
  OR2_X1 Inst_N_F2_U17 ( .A1(input1[1]), .A2(Inst_N_F2_n16), .ZN(Inst_N_F2_n14) );
  NAND2_X1 Inst_N_F2_U16 ( .A1(Inst_N_F2_n13), .A2(Inst_N_F2_n12), .ZN(
        Inst_N_F2_n15) );
  INV_X1 Inst_N_F2_U15 ( .A(input2[2]), .ZN(Inst_N_F2_n12) );
  NOR2_X1 Inst_N_F2_U14 ( .A1(Inst_N_F2_n11), .A2(Inst_N_F2_n10), .ZN(h[2]) );
  NOR2_X1 Inst_N_F2_U13 ( .A1(input1[1]), .A2(Inst_N_F2_n13), .ZN(
        Inst_N_F2_n10) );
  NOR2_X1 Inst_N_F2_U12 ( .A1(input1[0]), .A2(Inst_N_F2_n9), .ZN(Inst_N_F2_n13) );
  NAND2_X1 Inst_N_F2_U11 ( .A1(Inst_N_F2_n8), .A2(input2[2]), .ZN(
        Inst_N_F2_n11) );
  NAND2_X1 Inst_N_F2_U10 ( .A1(input1[1]), .A2(Inst_N_F2_n16), .ZN(
        Inst_N_F2_n8) );
  NAND2_X1 Inst_N_F2_U9 ( .A1(input1[0]), .A2(Inst_N_F2_n9), .ZN(Inst_N_F2_n16) );
  INV_X1 Inst_N_F2_U8 ( .A(input1[3]), .ZN(Inst_N_F2_n9) );
  NOR2_X1 Inst_N_F2_U7 ( .A1(Inst_N_F2_n7), .A2(Inst_N_F2_n6), .ZN(g[2]) );
  NAND2_X1 Inst_N_F2_U6 ( .A1(input1[1]), .A2(input1[0]), .ZN(Inst_N_F2_n6) );
  XNOR2_X1 Inst_N_F2_U5 ( .A(input1[3]), .B(input2[2]), .ZN(Inst_N_F2_n7) );
  NAND2_X1 Inst_N_F2_U4 ( .A1(Inst_N_F2_n16), .A2(Inst_N_F2_n5), .ZN(f[2]) );
  NAND2_X1 Inst_N_F2_U3 ( .A1(input2[2]), .A2(Inst_N_F2_n4), .ZN(Inst_N_F2_n5)
         );
  OR2_X1 Inst_N_F2_U2 ( .A1(input1[0]), .A2(Inst_N_F2_n3), .ZN(Inst_N_F2_n4)
         );
  XOR2_X1 Inst_N_F2_U1 ( .A(input1[1]), .B(input1[3]), .Z(Inst_N_F2_n3) );
  NOR2_X1 Inst_N_F3_U20 ( .A1(Inst_N_F3_n57), .A2(Inst_N_F3_n56), .ZN(f[3]) );
  XOR2_X1 Inst_N_F3_U19 ( .A(Inst_N_F3_n55), .B(input1[3]), .Z(Inst_N_F3_n56)
         );
  XNOR2_X1 Inst_N_F3_U18 ( .A(Inst_N_F3_n54), .B(input1[0]), .ZN(Inst_N_F3_n57) );
  NAND2_X1 Inst_N_F3_U17 ( .A1(Inst_N_F3_n53), .A2(Inst_N_F3_n52), .ZN(h[3])
         );
  NAND2_X1 Inst_N_F3_U16 ( .A1(input2[1]), .A2(Inst_N_F3_n54), .ZN(
        Inst_N_F3_n52) );
  NAND2_X1 Inst_N_F3_U15 ( .A1(input1[2]), .A2(Inst_N_F3_n51), .ZN(
        Inst_N_F3_n53) );
  NAND2_X1 Inst_N_F3_U14 ( .A1(Inst_N_F3_n50), .A2(Inst_N_F3_n49), .ZN(
        Inst_N_F3_n51) );
  OR2_X1 Inst_N_F3_U13 ( .A1(input2[1]), .A2(Inst_N_F3_n48), .ZN(Inst_N_F3_n50) );
  NAND2_X1 Inst_N_F3_U12 ( .A1(Inst_N_F3_n47), .A2(Inst_N_F3_n55), .ZN(g[3])
         );
  NAND2_X1 Inst_N_F3_U11 ( .A1(Inst_N_F3_n46), .A2(Inst_N_F3_n45), .ZN(
        Inst_N_F3_n55) );
  AND2_X1 Inst_N_F3_U10 ( .A1(input1[0]), .A2(input1[2]), .ZN(Inst_N_F3_n46)
         );
  NAND2_X1 Inst_N_F3_U9 ( .A1(Inst_N_F3_n44), .A2(input2[1]), .ZN(
        Inst_N_F3_n47) );
  XOR2_X1 Inst_N_F3_U8 ( .A(Inst_N_F3_n48), .B(Inst_N_F3_n54), .Z(
        Inst_N_F3_n44) );
  INV_X1 Inst_N_F3_U7 ( .A(input1[2]), .ZN(Inst_N_F3_n54) );
  NAND2_X1 Inst_N_F3_U6 ( .A1(Inst_N_F3_n43), .A2(Inst_N_F3_n42), .ZN(e[3]) );
  OR2_X1 Inst_N_F3_U5 ( .A1(Inst_N_F3_n45), .A2(Inst_N_F3_n49), .ZN(
        Inst_N_F3_n42) );
  NAND2_X1 Inst_N_F3_U4 ( .A1(input1[0]), .A2(input1[3]), .ZN(Inst_N_F3_n49)
         );
  INV_X1 Inst_N_F3_U3 ( .A(input2[1]), .ZN(Inst_N_F3_n45) );
  NAND2_X1 Inst_N_F3_U2 ( .A1(input1[2]), .A2(Inst_N_F3_n48), .ZN(
        Inst_N_F3_n43) );
  NOR2_X1 Inst_N_F3_U1 ( .A1(input1[0]), .A2(input1[3]), .ZN(Inst_N_F3_n48) );
  NAND2_X1 Inst_N_F4_U23 ( .A1(Inst_N_F4_n27), .A2(Inst_N_F4_n26), .ZN(f[4])
         );
  NAND2_X1 Inst_N_F4_U22 ( .A1(Inst_N_F4_n25), .A2(Inst_N_F4_n24), .ZN(
        Inst_N_F4_n26) );
  NAND2_X1 Inst_N_F4_U21 ( .A1(Inst_N_F4_n23), .A2(Inst_N_F4_n22), .ZN(
        Inst_N_F4_n27) );
  NAND2_X1 Inst_N_F4_U20 ( .A1(input1[0]), .A2(input2[3]), .ZN(Inst_N_F4_n22)
         );
  NAND2_X1 Inst_N_F4_U19 ( .A1(Inst_N_F4_n21), .A2(Inst_N_F4_n20), .ZN(
        Inst_N_F4_n23) );
  NAND2_X1 Inst_N_F4_U18 ( .A1(Inst_N_F4_n19), .A2(Inst_N_F4_n18), .ZN(e[4])
         );
  NAND2_X1 Inst_N_F4_U17 ( .A1(input1[0]), .A2(Inst_N_F4_n17), .ZN(
        Inst_N_F4_n18) );
  NAND2_X1 Inst_N_F4_U16 ( .A1(Inst_N_F4_n16), .A2(Inst_N_F4_n15), .ZN(
        Inst_N_F4_n19) );
  INV_X1 Inst_N_F4_U15 ( .A(input2[3]), .ZN(Inst_N_F4_n15) );
  NAND2_X1 Inst_N_F4_U14 ( .A1(Inst_N_F4_n14), .A2(Inst_N_F4_n13), .ZN(h[4])
         );
  NAND2_X1 Inst_N_F4_U13 ( .A1(Inst_N_F4_n12), .A2(input2[3]), .ZN(
        Inst_N_F4_n13) );
  NOR2_X1 Inst_N_F4_U12 ( .A1(input1[0]), .A2(Inst_N_F4_n20), .ZN(
        Inst_N_F4_n12) );
  NAND2_X1 Inst_N_F4_U11 ( .A1(input2[1]), .A2(Inst_N_F4_n17), .ZN(
        Inst_N_F4_n20) );
  INV_X1 Inst_N_F4_U10 ( .A(input2[2]), .ZN(Inst_N_F4_n17) );
  NAND2_X1 Inst_N_F4_U9 ( .A1(Inst_N_F4_n25), .A2(Inst_N_F4_n11), .ZN(
        Inst_N_F4_n14) );
  NOR2_X1 Inst_N_F4_U8 ( .A1(input2[1]), .A2(Inst_N_F4_n10), .ZN(Inst_N_F4_n25) );
  INV_X1 Inst_N_F4_U7 ( .A(input1[0]), .ZN(Inst_N_F4_n10) );
  NOR2_X1 Inst_N_F4_U6 ( .A1(Inst_N_F4_n16), .A2(Inst_N_F4_n9), .ZN(g[4]) );
  NAND2_X1 Inst_N_F4_U5 ( .A1(Inst_N_F4_n24), .A2(Inst_N_F4_n21), .ZN(
        Inst_N_F4_n9) );
  NAND2_X1 Inst_N_F4_U4 ( .A1(input2[2]), .A2(input2[3]), .ZN(Inst_N_F4_n21)
         );
  INV_X1 Inst_N_F4_U3 ( .A(Inst_N_F4_n11), .ZN(Inst_N_F4_n24) );
  NOR2_X1 Inst_N_F4_U2 ( .A1(input2[2]), .A2(input2[3]), .ZN(Inst_N_F4_n11) );
  OR2_X1 Inst_N_F4_U1 ( .A1(input1[0]), .A2(input2[1]), .ZN(Inst_N_F4_n16) );
  NOR2_X1 Inst_N_F5_U26 ( .A1(Inst_N_F5_n25), .A2(Inst_N_F5_n24), .ZN(g[5]) );
  AND2_X1 Inst_N_F5_U25 ( .A1(Inst_N_F5_n23), .A2(Inst_N_F5_n22), .ZN(
        Inst_N_F5_n25) );
  NAND2_X1 Inst_N_F5_U24 ( .A1(Inst_N_F5_n21), .A2(Inst_N_F5_n20), .ZN(f[5])
         );
  NOR2_X1 Inst_N_F5_U23 ( .A1(Inst_N_F5_n19), .A2(Inst_N_F5_n18), .ZN(
        Inst_N_F5_n20) );
  NOR2_X1 Inst_N_F5_U22 ( .A1(input1[1]), .A2(Inst_N_F5_n17), .ZN(
        Inst_N_F5_n18) );
  NAND2_X1 Inst_N_F5_U21 ( .A1(Inst_N_F5_n16), .A2(Inst_N_F5_n15), .ZN(
        Inst_N_F5_n21) );
  NAND2_X1 Inst_N_F5_U20 ( .A1(Inst_N_F5_n14), .A2(input1[2]), .ZN(
        Inst_N_F5_n15) );
  NAND2_X1 Inst_N_F5_U19 ( .A1(Inst_N_F5_n13), .A2(Inst_N_F5_n12), .ZN(
        Inst_N_F5_n16) );
  NAND2_X1 Inst_N_F5_U18 ( .A1(Inst_N_F5_n14), .A2(input1[1]), .ZN(
        Inst_N_F5_n12) );
  NAND2_X1 Inst_N_F5_U17 ( .A1(Inst_N_F5_n11), .A2(Inst_N_F5_n10), .ZN(
        Inst_N_F5_n14) );
  NAND2_X1 Inst_N_F5_U16 ( .A1(Inst_N_F5_n9), .A2(Inst_N_F5_n8), .ZN(e[5]) );
  NAND2_X1 Inst_N_F5_U15 ( .A1(input1[1]), .A2(Inst_N_F5_n22), .ZN(
        Inst_N_F5_n8) );
  NOR2_X1 Inst_N_F5_U14 ( .A1(Inst_N_F5_n19), .A2(Inst_N_F5_n7), .ZN(
        Inst_N_F5_n9) );
  NOR2_X1 Inst_N_F5_U13 ( .A1(input2[0]), .A2(Inst_N_F5_n13), .ZN(Inst_N_F5_n7) );
  NOR2_X1 Inst_N_F5_U12 ( .A1(Inst_N_F5_n23), .A2(Inst_N_F5_n10), .ZN(
        Inst_N_F5_n19) );
  NAND2_X1 Inst_N_F5_U11 ( .A1(input1[3]), .A2(Inst_N_F5_n13), .ZN(
        Inst_N_F5_n23) );
  INV_X1 Inst_N_F5_U10 ( .A(input1[2]), .ZN(Inst_N_F5_n13) );
  NAND2_X1 Inst_N_F5_U9 ( .A1(Inst_N_F5_n6), .A2(Inst_N_F5_n5), .ZN(h[5]) );
  NAND2_X1 Inst_N_F5_U8 ( .A1(input2[0]), .A2(Inst_N_F5_n4), .ZN(Inst_N_F5_n5)
         );
  NOR2_X1 Inst_N_F5_U7 ( .A1(input1[1]), .A2(Inst_N_F5_n22), .ZN(Inst_N_F5_n4)
         );
  NAND2_X1 Inst_N_F5_U6 ( .A1(input1[2]), .A2(Inst_N_F5_n11), .ZN(
        Inst_N_F5_n22) );
  INV_X1 Inst_N_F5_U5 ( .A(input1[3]), .ZN(Inst_N_F5_n11) );
  OR2_X1 Inst_N_F5_U4 ( .A1(Inst_N_F5_n17), .A2(Inst_N_F5_n24), .ZN(
        Inst_N_F5_n6) );
  NAND2_X1 Inst_N_F5_U3 ( .A1(input1[1]), .A2(Inst_N_F5_n10), .ZN(
        Inst_N_F5_n24) );
  INV_X1 Inst_N_F5_U2 ( .A(input2[0]), .ZN(Inst_N_F5_n10) );
  NAND2_X1 Inst_N_F5_U1 ( .A1(input1[3]), .A2(input1[2]), .ZN(Inst_N_F5_n17)
         );
  NAND2_X1 Inst_N_F6_U25 ( .A1(Inst_N_F6_n28), .A2(Inst_N_F6_n27), .ZN(h[6])
         );
  NAND2_X1 Inst_N_F6_U24 ( .A1(Inst_N_F6_n26), .A2(Inst_N_F6_n25), .ZN(
        Inst_N_F6_n27) );
  NAND2_X1 Inst_N_F6_U23 ( .A1(Inst_N_F6_n24), .A2(Inst_N_F6_n23), .ZN(
        Inst_N_F6_n25) );
  NOR2_X1 Inst_N_F6_U22 ( .A1(Inst_N_F6_n22), .A2(Inst_N_F6_n21), .ZN(
        Inst_N_F6_n28) );
  INV_X1 Inst_N_F6_U21 ( .A(Inst_N_F6_n20), .ZN(Inst_N_F6_n22) );
  NAND2_X1 Inst_N_F6_U20 ( .A1(Inst_N_F6_n19), .A2(Inst_N_F6_n18), .ZN(g[6])
         );
  NAND2_X1 Inst_N_F6_U19 ( .A1(Inst_N_F6_n17), .A2(Inst_N_F6_n20), .ZN(
        Inst_N_F6_n18) );
  NOR2_X1 Inst_N_F6_U18 ( .A1(Inst_N_F6_n26), .A2(Inst_N_F6_n24), .ZN(
        Inst_N_F6_n17) );
  NAND2_X1 Inst_N_F6_U17 ( .A1(Inst_N_F6_n16), .A2(Inst_N_F6_n24), .ZN(
        Inst_N_F6_n19) );
  NAND2_X1 Inst_N_F6_U16 ( .A1(Inst_N_F6_n15), .A2(Inst_N_F6_n20), .ZN(
        Inst_N_F6_n16) );
  NAND2_X1 Inst_N_F6_U15 ( .A1(input2[3]), .A2(Inst_N_F6_n14), .ZN(
        Inst_N_F6_n15) );
  NAND2_X1 Inst_N_F6_U14 ( .A1(Inst_N_F6_n13), .A2(Inst_N_F6_n12), .ZN(f[6])
         );
  NAND2_X1 Inst_N_F6_U13 ( .A1(input2[0]), .A2(Inst_N_F6_n11), .ZN(
        Inst_N_F6_n12) );
  NOR2_X1 Inst_N_F6_U12 ( .A1(Inst_N_F6_n21), .A2(Inst_N_F6_n10), .ZN(
        Inst_N_F6_n13) );
  NOR2_X1 Inst_N_F6_U11 ( .A1(Inst_N_F6_n23), .A2(Inst_N_F6_n14), .ZN(
        Inst_N_F6_n10) );
  INV_X1 Inst_N_F6_U10 ( .A(input1[1]), .ZN(Inst_N_F6_n14) );
  INV_X1 Inst_N_F6_U9 ( .A(input2[3]), .ZN(Inst_N_F6_n23) );
  NOR2_X1 Inst_N_F6_U8 ( .A1(input2[2]), .A2(Inst_N_F6_n9), .ZN(Inst_N_F6_n21)
         );
  NAND2_X1 Inst_N_F6_U7 ( .A1(input2[0]), .A2(input2[3]), .ZN(Inst_N_F6_n9) );
  NAND2_X1 Inst_N_F6_U6 ( .A1(Inst_N_F6_n20), .A2(Inst_N_F6_n8), .ZN(e[6]) );
  NAND2_X1 Inst_N_F6_U5 ( .A1(Inst_N_F6_n11), .A2(Inst_N_F6_n24), .ZN(
        Inst_N_F6_n8) );
  NOR2_X1 Inst_N_F6_U4 ( .A1(input2[3]), .A2(Inst_N_F6_n26), .ZN(Inst_N_F6_n11) );
  NOR2_X1 Inst_N_F6_U3 ( .A1(input2[2]), .A2(input1[1]), .ZN(Inst_N_F6_n26) );
  NAND2_X1 Inst_N_F6_U2 ( .A1(input2[2]), .A2(input1[1]), .ZN(Inst_N_F6_n20)
         );
  INV_X1 Inst_N_F6_U1 ( .A(input2[0]), .ZN(Inst_N_F6_n24) );
  NAND2_X1 Inst_N_F7_U24 ( .A1(Inst_N_F7_n27), .A2(Inst_N_F7_n26), .ZN(h[7])
         );
  NAND2_X1 Inst_N_F7_U23 ( .A1(Inst_N_F7_n25), .A2(input2[0]), .ZN(
        Inst_N_F7_n26) );
  NOR2_X1 Inst_N_F7_U22 ( .A1(input2[3]), .A2(Inst_N_F7_n24), .ZN(
        Inst_N_F7_n25) );
  NAND2_X1 Inst_N_F7_U21 ( .A1(input2[1]), .A2(Inst_N_F7_n23), .ZN(
        Inst_N_F7_n24) );
  NAND2_X1 Inst_N_F7_U20 ( .A1(Inst_N_F7_n22), .A2(Inst_N_F7_n21), .ZN(
        Inst_N_F7_n27) );
  NAND2_X1 Inst_N_F7_U19 ( .A1(Inst_N_F7_n19), .A2(Inst_N_F7_n18), .ZN(e[7])
         );
  NAND2_X1 Inst_N_F7_U18 ( .A1(input2[0]), .A2(Inst_N_F7_n22), .ZN(
        Inst_N_F7_n18) );
  NOR2_X1 Inst_N_F7_U17 ( .A1(Inst_N_F7_n17), .A2(Inst_N_F7_n16), .ZN(
        Inst_N_F7_n19) );
  NOR2_X1 Inst_N_F7_U16 ( .A1(Inst_N_F7_n22), .A2(Inst_N_F7_n15), .ZN(
        Inst_N_F7_n16) );
  INV_X1 Inst_N_F7_U15 ( .A(Inst_N_F7_n21), .ZN(Inst_N_F7_n15) );
  NOR2_X1 Inst_N_F7_U14 ( .A1(Inst_N_F7_n14), .A2(input2[0]), .ZN(
        Inst_N_F7_n21) );
  NOR2_X1 Inst_N_F7_U13 ( .A1(input2[1]), .A2(input1[2]), .ZN(Inst_N_F7_n22)
         );
  NOR2_X1 Inst_N_F7_U12 ( .A1(input2[0]), .A2(Inst_N_F7_n13), .ZN(
        Inst_N_F7_n17) );
  NAND2_X1 Inst_N_F7_U11 ( .A1(input2[1]), .A2(input1[2]), .ZN(Inst_N_F7_n13)
         );
  NOR2_X1 Inst_N_F7_U10 ( .A1(Inst_N_F7_n12), .A2(Inst_N_F7_n20), .ZN(g[7]) );
  INV_X1 Inst_N_F7_U9 ( .A(input2[0]), .ZN(Inst_N_F7_n20) );
  NOR2_X1 Inst_N_F7_U8 ( .A1(input2[1]), .A2(Inst_N_F7_n11), .ZN(Inst_N_F7_n12) );
  XOR2_X1 Inst_N_F7_U7 ( .A(Inst_N_F7_n23), .B(Inst_N_F7_n14), .Z(
        Inst_N_F7_n11) );
  INV_X1 Inst_N_F7_U6 ( .A(input2[3]), .ZN(Inst_N_F7_n14) );
  INV_X1 Inst_N_F7_U5 ( .A(input1[2]), .ZN(Inst_N_F7_n23) );
  NOR2_X1 Inst_N_F7_U4 ( .A1(Inst_N_F7_n9), .A2(Inst_N_F7_n10), .ZN(f[7]) );
  XOR2_X1 Inst_N_F7_U3 ( .A(input2[1]), .B(Inst_N_F7_n20), .Z(Inst_N_F7_n10)
         );
  XNOR2_X1 Inst_N_F7_U2 ( .A(Inst_N_F7_n8), .B(input2[3]), .ZN(Inst_N_F7_n9)
         );
  NOR2_X1 Inst_N_F7_U1 ( .A1(input1[2]), .A2(input2[0]), .ZN(Inst_N_F7_n8) );
  NOR2_X1 Inst_N_F8_U22 ( .A1(Inst_N_F8_n18), .A2(Inst_N_F8_n17), .ZN(h[8]) );
  NOR2_X1 Inst_N_F8_U21 ( .A1(input1[3]), .A2(Inst_N_F8_n16), .ZN(
        Inst_N_F8_n17) );
  NOR2_X1 Inst_N_F8_U20 ( .A1(input2[0]), .A2(Inst_N_F8_n15), .ZN(
        Inst_N_F8_n16) );
  NAND2_X1 Inst_N_F8_U19 ( .A1(Inst_N_F8_n14), .A2(input2[2]), .ZN(
        Inst_N_F8_n18) );
  NAND2_X1 Inst_N_F8_U18 ( .A1(input1[3]), .A2(Inst_N_F8_n13), .ZN(
        Inst_N_F8_n14) );
  INV_X1 Inst_N_F8_U17 ( .A(Inst_N_F8_n12), .ZN(Inst_N_F8_n13) );
  OR2_X1 Inst_N_F8_U16 ( .A1(Inst_N_F8_n11), .A2(Inst_N_F8_n10), .ZN(e[8]) );
  NOR2_X1 Inst_N_F8_U15 ( .A1(input1[3]), .A2(Inst_N_F8_n9), .ZN(Inst_N_F8_n10) );
  NOR2_X1 Inst_N_F8_U14 ( .A1(input2[2]), .A2(Inst_N_F8_n12), .ZN(Inst_N_F8_n9) );
  NOR2_X1 Inst_N_F8_U13 ( .A1(input2[1]), .A2(Inst_N_F8_n8), .ZN(Inst_N_F8_n12) );
  NOR2_X1 Inst_N_F8_U12 ( .A1(Inst_N_F8_n7), .A2(Inst_N_F8_n6), .ZN(
        Inst_N_F8_n11) );
  NOR2_X1 Inst_N_F8_U11 ( .A1(input2[1]), .A2(input2[2]), .ZN(Inst_N_F8_n6) );
  NAND2_X1 Inst_N_F8_U10 ( .A1(Inst_N_F8_n5), .A2(Inst_N_F8_n4), .ZN(f[8]) );
  NAND2_X1 Inst_N_F8_U9 ( .A1(input1[3]), .A2(Inst_N_F8_n3), .ZN(Inst_N_F8_n4)
         );
  NOR2_X1 Inst_N_F8_U8 ( .A1(input2[0]), .A2(input2[1]), .ZN(Inst_N_F8_n3) );
  NAND2_X1 Inst_N_F8_U7 ( .A1(Inst_N_F8_n7), .A2(Inst_N_F8_n2), .ZN(
        Inst_N_F8_n5) );
  NOR2_X1 Inst_N_F8_U6 ( .A1(Inst_N_F8_n15), .A2(Inst_N_F8_n8), .ZN(
        Inst_N_F8_n7) );
  NOR2_X1 Inst_N_F8_U5 ( .A1(Inst_N_F8_n8), .A2(Inst_N_F8_n1), .ZN(g[8]) );
  NOR2_X1 Inst_N_F8_U4 ( .A1(Inst_N_F8_n15), .A2(Inst_N_F8_n2), .ZN(
        Inst_N_F8_n1) );
  XNOR2_X1 Inst_N_F8_U3 ( .A(input2[2]), .B(input1[3]), .ZN(Inst_N_F8_n2) );
  INV_X1 Inst_N_F8_U2 ( .A(input2[1]), .ZN(Inst_N_F8_n15) );
  INV_X1 Inst_N_F8_U1 ( .A(input2[0]), .ZN(Inst_N_F8_n8) );
  DFF_X1 reg_GEN_s_current_state_reg_0_ ( .D(e[1]), .CK(clk), .Q(e_reg[1]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_1_ ( .D(e[2]), .CK(clk), .Q(e_reg[2]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_2_ ( .D(e[3]), .CK(clk), .Q(e_reg[3]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_3_ ( .D(e[4]), .CK(clk), .Q(e_reg[4]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_4_ ( .D(e[5]), .CK(clk), .Q(e_reg[5]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_5_ ( .D(e[6]), .CK(clk), .Q(e_reg[6]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_6_ ( .D(e[7]), .CK(clk), .Q(e_reg[7]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_7_ ( .D(e[8]), .CK(clk), .Q(e_reg[8]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_8_ ( .D(f[1]), .CK(clk), .Q(f_reg[1]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_9_ ( .D(f[2]), .CK(clk), .Q(f_reg[2]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_10_ ( .D(f[3]), .CK(clk), .Q(f_reg[3]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_11_ ( .D(f[4]), .CK(clk), .Q(f_reg[4]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_12_ ( .D(f[5]), .CK(clk), .Q(f_reg[5]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_13_ ( .D(f[6]), .CK(clk), .Q(f_reg[6]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_14_ ( .D(f[7]), .CK(clk), .Q(f_reg[7]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_15_ ( .D(f[8]), .CK(clk), .Q(f_reg[8]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_16_ ( .D(g[1]), .CK(clk), .Q(g_reg[1]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_17_ ( .D(g[2]), .CK(clk), .Q(g_reg[2]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_18_ ( .D(g[3]), .CK(clk), .Q(g_reg[3]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_19_ ( .D(g[4]), .CK(clk), .Q(g_reg[4]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_20_ ( .D(g[5]), .CK(clk), .Q(g_reg[5]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_21_ ( .D(g[6]), .CK(clk), .Q(g_reg[6]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_22_ ( .D(g[7]), .CK(clk), .Q(g_reg[7]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_23_ ( .D(g[8]), .CK(clk), .Q(g_reg[8]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_24_ ( .D(h[1]), .CK(clk), .Q(h_reg[1]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_25_ ( .D(h[2]), .CK(clk), .Q(h_reg[2]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_26_ ( .D(h[3]), .CK(clk), .Q(h_reg[3]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_27_ ( .D(h[4]), .CK(clk), .Q(h_reg[4]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_28_ ( .D(h[5]), .CK(clk), .Q(h_reg[5]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_29_ ( .D(h[6]), .CK(clk), .Q(h_reg[6]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_30_ ( .D(h[7]), .CK(clk), .Q(h_reg[7]), 
        .QN() );
  DFF_X1 reg_GEN_s_current_state_reg_31_ ( .D(h[8]), .CK(clk), .Q(h_reg[8]), 
        .QN() );
  XNOR2_X1 Inst_L_XORInst1_U3 ( .A(Inst_L_XORInst1_n6), .B(Inst_L_XORInst1_n5), 
        .ZN(output1[0]) );
  XNOR2_X1 Inst_L_XORInst1_U2 ( .A(e_reg[3]), .B(e_reg[4]), .ZN(
        Inst_L_XORInst1_n5) );
  XOR2_X1 Inst_L_XORInst1_U1 ( .A(e_reg[1]), .B(e_reg[2]), .Z(
        Inst_L_XORInst1_n6) );
  XNOR2_X1 Inst_L_XORInst2_U3 ( .A(Inst_L_XORInst2_n6), .B(Inst_L_XORInst2_n5), 
        .ZN(output2[0]) );
  XNOR2_X1 Inst_L_XORInst2_U2 ( .A(e_reg[7]), .B(e_reg[8]), .ZN(
        Inst_L_XORInst2_n5) );
  XOR2_X1 Inst_L_XORInst2_U1 ( .A(e_reg[5]), .B(e_reg[6]), .Z(
        Inst_L_XORInst2_n6) );
  XNOR2_X1 Inst_L_XORInst3_U3 ( .A(Inst_L_XORInst3_n6), .B(Inst_L_XORInst3_n5), 
        .ZN(output1[1]) );
  XNOR2_X1 Inst_L_XORInst3_U2 ( .A(f_reg[3]), .B(f_reg[4]), .ZN(
        Inst_L_XORInst3_n5) );
  XOR2_X1 Inst_L_XORInst3_U1 ( .A(f_reg[1]), .B(f_reg[2]), .Z(
        Inst_L_XORInst3_n6) );
  XNOR2_X1 Inst_L_XORInst4_U3 ( .A(Inst_L_XORInst4_n6), .B(Inst_L_XORInst4_n5), 
        .ZN(output2[1]) );
  XNOR2_X1 Inst_L_XORInst4_U2 ( .A(f_reg[7]), .B(f_reg[8]), .ZN(
        Inst_L_XORInst4_n5) );
  XOR2_X1 Inst_L_XORInst4_U1 ( .A(f_reg[5]), .B(f_reg[6]), .Z(
        Inst_L_XORInst4_n6) );
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

