#!/bin/csh
# Name: LI Chi
# ID: 11808749d
set card = (SA S2 S3 S4 S5 S6 S7 S8 S9 ST SJ SQ SK HA H2 H3 H4 H5 H6 H7 H8 H9 HT HJ HQ HK CA C2 C3 C4 C5 C6 C7 C8 C9 CT CJ CQ CK DA D2 D3 D4 D5 D6 D7 D8 D9 DT DJ DQ DK)
echo "Player 1 holds $card[$argv[1]] $card[$argv[2]] $card[$argv[3]] $card[$argv[4]] $card[$argv[5]]"
echo "Player 2 holds $card[$argv[6]] $card[$argv[7]] $card[$argv[8]] $card[$argv[9]] $card[$argv[10]]"
set temp 
set newcard
set i = 1

# transfer the card to number 0-12 which is A-K
while ($i <= 10)
	@ temp =( $argv[$i] - 1) % 13
	set newcard = ($newcard $temp)
	@ i++
end
set number
@ i = 1
while ( $i < 6)
	@ temp = 0
	foreach new ($newcard[1-5])
		if ($new == $newcard[$i]) then
			@ temp++
		endif
	end
	@ i++
	set number = ($number $temp)
end

#check the number of same card
@ check4  = 0 check3  = 0 check2 = 0 
@ i = 1
while ( $i < 6)
	switch  ($number[$i])
	case 4:
		@ check4 = 1
		breaksw
	case 3:
		@ check3 = 1
		breaksw
	case 2:
		@ check2 = 1
		breaksw
	endsw
	@ i++
end

if ($check4 == 1) then
	echo "Player 1 holds four of king"
else if ($check3 == 1) then
	if ($check2 == 1) then 
		echo "Player 1 holds full house"
	else
		echo "Player 1 holds nothing"
	endif
else 
	echo "Player 1 holds nothing"
endif

# same as player1	
@ i = 6
while ( $i < 11)
	@ temp = 0
	foreach new ($newcard[6-10])
		if ($new == $newcard[$i]) then
			@ temp++
		endif
	end
	@ i++
	set number = ($number $temp)
end

@ check24  = 0 check23  = 0 check22 = 0 

@ i = 6
while ( $i < 11)
	switch  ($number[$i])
	case 4:
		@ check24 = 1
		breaksw
	case 3:
		@ check23 = 1
		breaksw
	case 2:
		@ check22 = 1
		breaksw
	endsw
	@ i++
end

if ($check24 == 1) then
	echo "Player 2 holds four of king"
else if ($check23 == 1) then
	if ($check22 == 1) then 
		echo "Player 2 holds full house"
	else
		echo "Player 2 holds nothing"
	endif
else 
	echo "Player 2 holds nothing"
endif

# bubble sort player1
@ i=1 j=5
while ( $i < 6)
	@ j = 5
	while ($j > 1)
		@ p = $j - 1
		if ($newcard[$j] < $newcard[$p]) then
			@ temp = $newcard[$j]
			@ newcard[$j] = $newcard[$p]
			@ newcard[$p] = $temp
		endif
		@ j--
	end
	@ i++
end
set newcard2 = ($newcard[6-10])

# bubble sort player2
@ i=1 j=5
while ( $i < 6)
	@ j = 5
	while ($j > 1)
		@ p = $j - 1
		if ($newcard2[$j] < $newcard2[$p]) then
			@ temp = $newcard2[$j]
			@ newcard2[$j] = $newcard2[$p]
			@ newcard2[$j] = $temp
		endif
		@ j--
	end
	@ i++
end


if ($check4 > $check24) then 
	echo "Player 1 is winner"
	exit(0)
else if ($check4 < $check24) then
	echo "Player 2 is winner"
	exit(0)
endif

if ($check3 > $check23 && $check2 > $check22) then 
	echo "Player 1 is winner"
	exit(0)
else if ($check3 < $check23 && $check2 < $check22) then
	echo "Player 2 is winner"
	exit(0)
endif

@ i=5 j=1
while ( $i > 0)
	if ($newcard[$i] > $newcard2[$i]) then
		echo "Player 1 is winner"
		exit(0)
	else if ($newcard[$i] < $newcard2[$i]) then
		echo "Player 2 is winner"
		exit(0)
	endif
	@ i++
end


