;; **************************************************************
;; ==============================================================
;;                  COMPORTAMENTOS ELEMENTARES
;; KEY-PRESS: OK 
;; ==============================================================
;; **************************************************************


;; ==============================================================
;; KEY-PRESS
;; ==============================================================
(add-dm (geh-key-adjust-position isa chunk))
;; Produção chamada para pressionar uma tecla, neste estado posicionamos a mão
(p P-GEH-KEY-PRESS-POSITIONING
	=goal>
		isa		task-step
		type	geh-key-press
		state	do-this
		order	=order
		key		=key
	?manual>
		state	free
==>
	; !output!(pressionando uma tecla)
	=goal>
		state	geh-key-adjust-position
		order	=order
	+manual>
		isa		point-hand-at-key
		to-key	=key
		hand	right
)

;; Produção do ato de pressionar uma tecla
(p P-GEH-KEY-PRESS-ACT
	=goal>
		isa		task-step
		type	geh-key-press
		state	geh-key-adjust-position
		key		=key
		order	=order
	?manual>
		state	free
==>
	!bind!	=next (+ 1 =order)
	=goal>
		isa		task-step
		type	nil
		state	do-this
		order	=next
	+manual>
		isa		press-key
		key		=key
)