; source: https://slideplayer.com/slide/13077285/

N 	equ $13 
	org $20 
sum 	rmb 2 
	org $C000 
	ldaa #$00 
	staa sum ; initialize sum to 0 
	staa sum+1 ; “ 
	ldx #$00 ; point X to array[0] 
loop 
	brclr 0,X $01 chkend ; is it an odd number? 
	ldd sum ; add the odd number to the sum 
	addb 0,X ; “ 
	adca #0 ; “ 
	std sum ; “ 
chkend 
	cpx #N ; compare the pointer to the address of the last element 
	bhs exit ; is this the end? 
	inx bra loop ; not yet done, continue 
exit 
	end
