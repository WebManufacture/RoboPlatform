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

         #11-18(1x) #dstAddr ...,  #srcAddr..., #size, #pmo,#pm, #value,...,#pms, #pm, value..., #pme, #pme, (#crc) ? #03
         x - XRT packet type

         #addr (#dstAddr, #srcAddr)
         First 1bits of address means this is end of address chain
         0 - packet chain
         1 - end packet
         Special bytes in packet chain may have sense -- delimeter, mark of the address type

         Для пакетов #SYNC
         #A0-#A9 - #Req
         #B0-#B9 - #Ack
         #C0-#C9 - Reserved
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

            1 Hi (Hi, when raise node)
            2 Dying (Something wrong, buy or error, Unscribe, Del)
            3 Acknowledge (Response, for anything, for example "see you as")
            4 Things (Get Description, Get anything, Add, Set)
            5 Lookup (All, Subscribe)
            6 Redirect (Info)
            7 Forward  (Note)
            8 Tunnel

            */
}
