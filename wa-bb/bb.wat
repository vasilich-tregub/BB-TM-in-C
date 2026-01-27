;; TERMS OF USE
;; This source code is subject to the terms of the MIT License. 
;; Copyright(c) 2026 Vladimir Vasilich Tregub
(module
	;; Create and export an wasm memory
	(memory $tapepos 1)
	(export "tapepos" (memory $tapepos))
	(memory $ruleArr 1)
	(export "waTMruleArr" (memory $ruleArr))
	(memory $ruleScrArr 1)
	(export "waTMruleScrArr" (memory $ruleScrArr))
	(memory $ruleBidirArr 1)
	(export "waTMruleBidirArr" (memory $ruleBidirArr))
	(memory $ruleStateArr 1)
	(export "waTMruleStateArr" (memory $ruleStateArr))

	(func (export "startWaTM") (result i64) 
		(local $POS i32)
		(local $tmrule i32)
		(local $ruleix i32) ;; rule index = ('[A..H]' - 'A')
		(local $shifts i32)
		(memory.fill (memory $tapepos) (i32.const 0) (i32.const 0) (i32.const 0x10000))
		(local.set $POS (i32.const 0x8000))
		(local.set $tmrule (i32.const 0))
		(local.set $shifts (i32.const 0))

		(loop $rulehandler
		(local.set $ruleix (i32.rotl (local.get $tmrule) (i32.const 3))) ;; ruleix offset in bytes (x4) and (x2) as state contains two rules
		(i32.eq (i32.load8_u (memory $tapepos) (local.get $POS)) (i32.const 0x31))
		(if 
			(then
				(local.set $ruleix (i32.add (local.get $ruleix) (i32.const 4)))
			)			
			(else)
		)
		(local.set $tmrule (i32.load (memory $ruleStateArr) (local.get $ruleix))) ;; set next-rule part of the instruction word
		(local.get $POS)		
		(i32.or (i32.load (memory $ruleScrArr) (local.get $ruleix)) (i32.const 0x30))
		(i32.store8 (memory $tapepos))
		(local.get $POS)
		(i32.load (memory $ruleBidirArr) (local.get $ruleix))
		(i32.add)
		(local.tee $POS)
		(i32.const 0xFFFF0000)
		(i32.and)
		(if
			(then
				(local.set $tmrule (i32.const 7))
			)
		)
		(local.set $shifts (i32.add (local.get $shifts) (i32.const 1)))
		(i32.ne (local.get $tmrule) (i32.const 7))
		(br_if $rulehandler)
		)
		(i64.shl (i64.extend_i32_u (local.get $shifts)) (i64.const 32))
		(i64.extend_i32_u (local.get $POS))
		(i64.or)
		return
	)
)