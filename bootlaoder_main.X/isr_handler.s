    .text
    .global __OscillatorFail
__OscillatorFail:
    btsc.b _mainApp,#0
    goto 0x5000 + 8 
    nop

    .global __AddressError
__AddressError:
    btsc.b _mainApp,#0
    goto 0x5000 + 12
    nop

    .global __HardTrapError
__HardTrapError:
    btsc.b _mainApp,#0
    goto 0x5000 + 16 
    nop

    .global __StackError
__StackError:
    btsc.b _mainApp,#0
    goto 0x5000 + 20 
    nop

    .global __MathError
__MathError:
    btsc.b _mainApp,#0
    goto 0x5000 + 24 
    nop

    .global __DMACError
__DMACError:
    btsc.b _mainApp,#0
    goto 0x5000 + 28 
    nop

    .global __SoftTrapError
__SoftTrapError:
    btsc.b _mainApp,#0
    goto 0x5000 + 32 
    nop

     .global __ReservedTrap7
__ReservedTrap7:
    btsc.b _mainApp,#0
    goto 0x5000 + 36 
    nop

    .global __INT0Interrupt
__INT0Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 40 
    nop
    
    .global __IC1Interrupt
__IC1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 44 
    nop

    .global __OC1Interrupt
__OC1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 48 
    nop

    .global __T1Interrupt
__T1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 52 
    nop

    .global __DMA0Interrupt
__DMA0Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 56 
    nop

    .global __IC2Interrupt
__IC2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 60 
    nop

    .global __OC2Interrupt
__OC2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 64
    nop

     .global __T2Interrupt
__T2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 68 
    nop

    .global __T3Interrupt
__T3Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 72 
    goto _T3Interrupt
    
    .global __SPI1ErrInterrupt
__SPI1ErrInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 76 
    nop

    .global __SPI1Interrupt
__SPI1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 80 
    nop

    .global __U1RXInterrupt
__U1RXInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 84
    goto _U1RXInterrupt

    .global __U1TXInterrupt
__U1TXInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 88
    nop

    .global __AD1Interrupt
__AD1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 92 
    nop

    .global __DMA1Interrupt
__DMA1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 96 
    nop

    .global __Interrupt15
__Interrupt15:
    btsc.b _mainApp,#0
    goto 0x5000 + 100 
    nop

    .global __SI2C1Interrupt
__SI2C1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 104 
    nop

    .global __MI2C1Interrupt
__MI2C1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 108 
    nop

    .global __CM1Interrupt
__CM1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 112 
    nop

    .global __CNInterrupt
__CNInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 116 
    nop

    .global __INT1Interrupt
__INT1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 120
    nop

    .global __Interrupt21
__Interrupt21:
    btsc.b _mainApp,#0
    goto 0x5000 + 124
    nop

    .global __Interrupt22
__Interrupt22:
    btsc.b _mainApp,#0
    goto 0x5000 + 128
    nop

    .global __Interrupt23
__Interrupt23:
    btsc.b _mainApp,#0
    goto 0x5000 + 132 
    nop

    .global __DMA2Interrupt
__DMA2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 136 
    nop

    .global __OC3Interrupt
__OC3Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 140
    nop

    .global __OC4Interrupt
__OC4Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 144 
    nop

    .global __T4Interrupt
__T4Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 148 
    nop

    .global __T5Interrupt
__T5Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 152 
    nop

    .global __INT2Interrupt
__INT2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 156 
    nop

    .global __U2RXInterrupt
__U2RXInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 160 
    nop

    .global __U2TXInterrupt
__U2TXInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 164 
    nop

    .global __SPI2ErrInterrupt
__SPI2ErrInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 168 
    nop

    .global __SPI2Interrupt
__SPI2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 172
    nop

    .global __C1RxRdyInterrupt
__C1RxRdyInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 176 
    nop

    .global __C1Interrupt
__C1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 180 
    nop

    .global __DMA3Interrupt
__DMA3Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 184 
    nop

    .global __IC3Interrupt
__IC3Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 188 
    nop

    .global __IC4Interrupt
__IC4Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 192 
    nop

    .global __Interrupt39
__Interrupt39:
    btsc.b _mainApp,#0
    goto 0x5000 + 196 
    nop

    .global __Interrupt40
__Interrupt40:
    btsc.b _mainApp,#0
    goto 0x5000 + 200 
    nop

    .global __Interrupt41
__Interrupt41:
    btsc.b _mainApp,#0
    goto 0x5000 + 204 
    nop

    .global __Interrupt42
__Interrupt42:
    btsc.b _mainApp,#0
    goto 0x5000 + 208 
    nop

    .global __Interrupt43
__Interrupt43:
    btsc.b _mainApp,#0
    goto 0x5000 + 212 
    nop

    .global __Interrupt45
__Interrupt45:
    btsc.b _mainApp,#0
    goto 0x5000 + 216 
    nop

    .global __Interrupt46
__Interrupt46:
    btsc.b _mainApp,#0
    goto 0x5000 + 220 
    nop

    .global __Interrupt47
__Interrupt47:
    btsc.b _mainApp,#0
    goto 0x5000 + 224 
    nop

    .global __Interrupt48
__Interrupt48:
    btsc.b _mainApp,#0
    goto 0x5000 + 228 
    nop

    .global __SI2C2Interrupt
__SI2C2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 232 
    nop

    .global __MI2C2Interrupt
__MI2C2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 236 
    nop

    .global __Interrupt51
__Interrupt51:
    btsc.b _mainApp,#0
    goto 0x5000 + 240 
    nop

    .global __Interrupt52
__Interrupt52:
    btsc.b _mainApp,#0
    goto 0x5000 + 244 
    nop

    .global __Interrupt53
__Interrupt53:
    btsc.b _mainApp,#0
    goto 0x5000 + 248 
    nop

    .global __Interrupt54
__Interrupt54:
    btsc.b _mainApp,#0
    goto 0x5000 + 252 
    nop

    .global __Interrupt55
__Interrupt55:
    btsc.b _mainApp,#0
    goto 0x5000 + 256 
    nop

    .global __Interrupt56
__Interrupt56:
    btsc.b _mainApp,#0
    goto 0x5000 + 260 
    nop

    .global __Interrupt57
__Interrupt57:
    btsc.b _mainApp,#0
    goto 0x5000 + 264 
    nop

    .global __Interrupt58
__Interrupt58:
    btsc.b _mainApp,#0
    goto 0x5000 + 268 
    nop

    .global __Interrupt59
__Interrupt59:
    btsc.b _mainApp,#0
    goto 0x5000 + 272 
    nop

    .global __Interrupt60
__Interrupt60:
    btsc.b _mainApp,#0
    goto 0x5000 + 276 
    nop

    .global __Interrupt61
__Interrupt61:
    btsc.b _mainApp,#0
    goto 0x5000 + 280 
    nop

    .global __Interrupt62
__Interrupt62:
    btsc.b _mainApp,#0
    goto 0x5000 + 284 
    nop

    .global __Interrupt63
__Interrupt63:
    btsc.b _mainApp,#0
    goto 0x5000 + 288 
    nop

    .global __Interrupt64
__Interrupt64:
    btsc.b _mainApp,#0
    goto 0x5000 + 292 
    nop

    .global __U1ErrInterrupt
__U1ErrInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 296 
    nop

    .global __U2ErrInterrupt
__U2ErrInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 300 
    nop

    .global __CRCInterrupt
__CRCInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 304 
    nop

    .global __Interrupt68
__Interrupt68:
    btsc.b _mainApp,#0
    goto 0x5000 + 308 
    nop

    .global __Interrupt69
__Interrupt69:
    btsc.b _mainApp,#0
    goto 0x5000 + 312 
    nop

    .global __C1TxReqInterrupt
__C1TxReqInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 316 
    nop

    .global __Interrupt71
__Interrupt71:
    btsc.b _mainApp,#0
    goto 0x5000 + 320
    nop

    .global __Interrupt72
__Interrupt72:
    btsc.b _mainApp,#0
    goto 0x5000 + 324 
    nop

    .global __Interrupt73
__Interrupt73:
    btsc.b _mainApp,#0
    goto 0x5000 + 328 
    nop

    .global __Interrupt74
__Interrupt74:
    btsc.b _mainApp,#0
    goto 0x5000 + 332 
    nop
    
    .global __Interrupt75
__Interrupt75:
    btsc.b _mainApp,#0
    goto 0x5000 + 336 
    nop

    .global __Interrupt76
__Interrupt76:
    btsc.b _mainApp,#0
    goto 0x5000 + 340 
    nop

    .global __CTMUInterrupt
__CTMUInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 344 
    nop

    .global __Interrupt78
__Interrupt78:
    btsc.b _mainApp,#0
    goto 0x5000 + 348 
    nop
    
    .global __Interrupt79
__Interrupt79:
    btsc.b _mainApp,#0
    goto 0x5000 + 352 
    nop

    .global __Interrupt80
__Interrupt80:
    btsc.b _mainApp,#0
    goto 0x5000 + 356 
    nop

    .global __Interrupt81
__Interrupt81:
    btsc.b _mainApp,#0
    goto 0x5000 + 360 
    nop

    .global __Interrupt82
__Interrupt82:
    btsc.b _mainApp,#0
    goto 0x5000 + 364 
    nop

    .global __Interrupt83
__Interrupt83:
    btsc.b _mainApp,#0
    goto 0x5000 + 368 
    nop

    .global __Interrupt84
__Interrupt84:
    btsc.b _mainApp,#0
    goto 0x5000 + 372 
    nop

    .global __Interrupt85
__Interrupt85:
    btsc.b _mainApp,#0
    goto 0x5000 + 376 
    nop

    .global __Interrupt86
__Interrupt86:
    btsc.b _mainApp,#0
    goto 0x5000 + 380 
    nop

    .global __Interrupt87
__Interrupt87:
    btsc.b _mainApp,#0
    goto 0x5000 + 384 
    nop

    .global __Interrupt88
__Interrupt88:
    btsc.b _mainApp,#0
    goto 0x5000 + 388 
    nop
    
    .global __Interrupt89
__Interrupt89:	
    btsc.b _mainApp,#0
    goto 0x5000 + 392 
    nop

    .global __Interrupt90
__Interrupt90:
    btsc.b _mainApp,#0
    goto 0x5000 + 396 
    nop
    
    .global __Interrupt91
__Interrupt91:
    btsc.b _mainApp,#0
    goto 0x5000 + 400 
    nop

    .global __Interrupt92
__Interrupt92:
    btsc.b _mainApp,#0
    goto 0x5000 + 404 
    nop

    .global __Interrupt93
__Interrupt93:
    btsc.b _mainApp,#0
    goto 0x5000 + 408 
    nop

    .global __Interrupt94
__Interrupt94:
    btsc.b _mainApp,#0
    goto 0x5000 + 412 
    nop

    .global __Interrupt95
__Interrupt95:
    btsc.b _mainApp,#0
    goto 0x5000 + 416 
    nop

    .global __Interrupt96
__Interrupt96:
    btsc.b _mainApp,#0
    goto 0x5000 + 420 
    nop
    
    .global __Interrupt97
__Interrupt97:
    btsc.b _mainApp,#0
    goto 0x5000 + 424 
    nop

    .global __Interrupt98
__Interrupt98:
    btsc.b _mainApp,#0
    goto 0x5000 + 428 
    nop

    .global __Interrupt99
__Interrupt99:
    btsc.b _mainApp,#0
    goto 0x5000 + 432 
    nop

    .global __Interrupt100
__Interrupt100:
    btsc.b _mainApp,#0
    goto 0x5000 + 436 
    nop

    .global __Interrupt101
__Interrupt101:
    btsc.b _mainApp,#0
    goto 0x5000 + 440 
    nop

    .global __Interrupt102
__Interrupt102:
    btsc.b _mainApp,#0
    goto 0x5000 + 444 
    nop

    .global __Interrupt103
__Interrupt103:
    btsc.b _mainApp,#0
    goto 0x5000 + 448 
    nop

    .global __Interrupt104
__Interrupt104:
    btsc.b _mainApp,#0
    goto 0x5000 + 452 
    nop

    .global __Interrupt105
__Interrupt105:
    btsc.b _mainApp,#0
    goto 0x5000 + 456 
    nop

    .global __Interrupt106
__Interrupt106:
    btsc.b _mainApp,#0
    goto 0x5000 + 460 
    nop

    .global __Interrupt107
__Interrupt107:
    btsc.b _mainApp,#0
    goto 0x5000 + 464 
    nop

    .global __Interrupt108
__Interrupt108:
    btsc.b _mainApp,#0
    goto 0x5000 + 468 
    nop

    .global __Interrupt109
__Interrupt109:
    btsc.b _mainApp,#0
    goto 0x5000 + 472 
    nop

    .global __Interrupt110
__Interrupt110:
    btsc.b _mainApp,#0
    goto 0x5000 + 476 
    nop

    .global __Interrupt111
__Interrupt111:
    btsc.b _mainApp,#0
    goto 0x5000 + 480 
    nop

    .global __Interrupt112
__Interrupt112:
    btsc.b _mainApp,#0
    goto 0x5000 + 484 
    nop

    .global __Interrupt113
__Interrupt113:
    btsc.b _mainApp,#0
    goto 0x5000 + 488 
    nop
    .global __Interrupt114
__Interrupt114:
    btsc.b _mainApp,#0
    goto 0x5000 + 492 
    nop

    .global __Interrupt115
__Interrupt115:
    btsc.b _mainApp,#0
    goto 0x5000 + 496 
    nop

    .global __Interrupt116
__Interrupt116:
    btsc.b _mainApp,#0
    goto 0x5000 + 500 
    nop

    .global __Interrupt117
__Interrupt117:
    btsc.b _mainApp,#0
    goto 0x5000 + 504 
    nop

    .global __Interrupt118
__Interrupt118:
    btsc.b _mainApp,#0
    goto 0x5000 + 508 
    nop

    .global __Interrupt119
__Interrupt119:
    btsc.b _mainApp,#0
    goto 0x5000 + 512 
    nop

    .global __Interrupt120
__Interrupt120:
    btsc.b _mainApp,#0
    goto 0x5000 + 516 
    nop

    .global __Interrupt121
__Interrupt121:
    btsc.b _mainApp,#0
    goto 0x5000 + 520 
    nop

    .global __Interrupt122
__Interrupt122:
    btsc.b _mainApp,#0
    goto 0x5000 + 524 
    nop

    .global __Interrupt123
__Interrupt123:
    btsc.b _mainApp,#0
    goto 0x5000 + 528
    nop
 
    .global __Interrupt124
__Interrupt124:
    btsc.b _mainApp,#0
    goto 0x5000 + 532 
    nop

    .global __Interrupt125
__Interrupt125:
    btsc.b _mainApp,#0
    goto 0x5000 + 536 
    nop

    .global __Interrupt126
__Interrupt126:
    btsc.b _mainApp,#0
    goto 0x5000 + 540 
    nop

    .global __Interrupt127
__Interrupt127:
    btsc.b _mainApp,#0
    goto 0x5000 + 544 
    nop

    .global __Interrupt128
__Interrupt128:
    btsc.b _mainApp,#0
    goto 0x5000 + 548 
    nop

    .global __Interrupt129
__Interrupt129:
    btsc.b _mainApp,#0
    goto 0x5000 + 552 
    nop

    .global __Interrupt130
__Interrupt130:
    btsc.b _mainApp,#0
    goto 0x5000 + 556 
    nop
    
    .global __Interrupt131
__Interrupt131:
    btsc.b _mainApp,#0
    goto 0x5000 + 560 
    nop

    .global __Interrupt132
__Interrupt132:
    btsc.b _mainApp,#0
    goto 0x5000 + 564 
    nop

    .global __Interrupt133
__Interrupt133:
    btsc.b _mainApp,#0
    goto 0x5000 + 568 
    nop

    .global __Interrupt134
__Interrupt134:
    btsc.b _mainApp,#0
    goto 0x5000 + 572 
    nop

    .global __Interrupt135
__Interrupt135:
    btsc.b _mainApp,#0
    goto 0x5000 + 576 
    nop

    .global __Interrupt136
__Interrupt136:
    btsc.b _mainApp,#0
    goto 0x5000 + 580 
    nop

    .global __Interrupt137
__Interrupt137:
    btsc.b _mainApp,#0
    goto 0x5000 + 584 
    nop

    .global __Interrupt138
__Interrupt138:
    btsc.b _mainApp,#0
    goto 0x5000 + 588 
    nop

    .global __Interrupt139
__Interrupt139:
    btsc.b _mainApp,#0
    goto 0x5000 + 592 
    nop

    .global __Interrupt140
__Interrupt140:
    btsc.b _mainApp,#0
    goto 0x5000 + 596 
    nop

    .global __Interrupt141
__Interrupt141:
    btsc.b _mainApp,#0
    goto 0x5000 + 600 
    nop
    
    .global __ICDInterrupt
__ICDInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 604 
    nop

    .global __JTAGInterrupt
__JTAGInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 608 
    nop

    .global __Interrupt144
__Interrupt144:
    btsc.b _mainApp,#0
    goto 0x5000 + 612 
    nop

    .global __PTGSTEPInterrupt
__PTGSTEPInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 616 
    nop

    .global __PTGWDTInterrupt
__PTGWDTInterrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 620 
    nop

    .global __PTG0Interrupt
__PTG0Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 624 
    nop

    .global __PTG1Interrupt
__PTG1Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 628 
    nop

    .global __PTG2Interrupt
__PTG2Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 632 
    nop

    .global __PTG3Interrupt
__PTG3Interrupt:
    btsc.b _mainApp,#0
    goto 0x5000 + 636 
    nop






