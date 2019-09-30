{
    Raw = 0,
        Simple = 1,      //UNSIZED               #02 ... data ... #03
        SimpleCoded = 2, //UNSIZED               coded by 2bytes ($3D = #F3 #FD) data      #02 ... coded data ... #03
        SimpleCRC = 3,   //UNSIZED               WITH CRC  #02 ... data ... #crc #03
        PacketInvariant = 5,// CAN BE 2, 3, 10, 11, 14, 15
        Sized = 10,      //SIZED  #01 #size #02? ... data ... #03
        SizedOld = 11,   //SIZED  #01 #size #02? ... data ... #04
        SizedCRC = 14,   //SIZED  #01 #size #02? ... data ... #CRC #03
        SizedCRC_old = 15,   //SIZED  #01 #size ... data ... #CRC #04
        Addressed = 20,  //With addr  #01 #size #addr ... data ... #03
        AddressedOld = 21,  //With addr  #01 #size #addr ... data ... #04
        XRouting = 30,    //XRouting addressed  #01 #size #dstAddr #dstType #srcAddr #srcType ... data ... #crc #03
        LIN = 32,    //XRouting addressed  #01 #size #02 #dstAddr #dstType #srcAddr #srcType ... data ... #crc #03
        XRoutingObject=40
        /*XRouting object addressed

         Для пакетов XRT кроме THINGS, REDIRECT, FORWARD
         #11-18(except 17,18) #dstAddr1 #dstAddr2 #srcAddr1 #srcAddr2 #size #pmo,#pm, #value,...,#pms, #pm, value..., #pme, #pme, #03
         Где x - тип пакета #xrt

         Для пакетов THINGS, REDIRECT, FORWARD
         #14,16-17 #dstLocalAddr #srcLocalAddr #identifier #size #pmo,#pm, #value,...,#pms, #pm, value..., #pme, #pme, #03

         Для пакетов #SYNC
         #A0-#CF - зарезервировано
         #E0-#EF #syncData1 #syncData2
         #F0-FF #syncData (1Byte)

         */

           /*pm = packet marker ( 3bit - dataType + 5bit thing identifier )
              pm data types:

              00 - Raw Data, for a strings or something encoded, looking the next PM
              01 - Sized    something have fixed size, things identifier describe size
              10 - #pms Complex start with another PM's
              11 - #pme Complex End


            */
           /*
            XRT packet type:

            1 Hi (Subscribe)
            2 Buy (Unscribe)
            3 SeeYou (Accepted)
            4 Things (List)
            5 Lookup (Get Description)
            6 Redirect (Emit)
            7 Forward  (Emit)
            8 Tunnel

            */
}
