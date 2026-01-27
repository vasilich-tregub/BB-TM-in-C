/* TERMS OF USE
 * This source code is subject to the terms of the MIT License. 
 * Copyright(c) 2026 Vladimir Vasilich Tregub
*/

// 2 symbol Turing Machine
const STATE_A = 'A';
const STATE_B = 'B';
const STATE_C = 'C';
const STATE_D = 'D';
const STATE_E = 'E';
const STATE_F = 'F';
const STATE_H = 'H';

window.onload = () => {
    const radios = document.querySelectorAll('input[name="lastState"]');
    const allStateCodes = document.querySelectorAll('input[name="stateCodes"]');

    radios.forEach(radio => {
        radio.addEventListener('change', event => {
            allStateCodes.forEach(stateCode => {
                stateCode.value = '';
            });
            idShifts.innerHTML = "Shifts:"
            idTapeDisplay.innerHTML = "Tape content:"
            tmstate = STATE_A;
            switch (event.target.value) { 
                case '1':
                    idState0A.value = '1RH';
                    break;
                case '2':
                    idState0A.value = '1RB';
                    idState1A.value = '1LB';
                    idState0B.value = '1LA';
                    idState1B.value = '1RH';
                    break;
                case '3':
                    idState0A.value = '1RB';
                    idState1A.value = '1RH';
                    idState0B.value = '0RC';
                    idState1B.value = '1RB';
                    idState0C.value = '1LC';
                    idState1C.value = '1LA';
                    break;
                case '4':
                    idState0A.value = '1RB';
                    idState1A.value = '1LB';
                    idState0B.value = '1LA';
                    idState1B.value = '0LC';
                    idState0C.value = '1RH';
                    idState1C.value = '1LD';
                    idState0D.value = '1RD';
                    idState1D.value = '0RA';
                    break;
                case '5':
                    idState0A.value = '1RB';
                    idState1A.value = '1LC';
                    idState0B.value = '1RC';
                    idState1B.value = '1RB';
                    idState0C.value = '1RD';
                    idState1C.value = '0LE';
                    idState0D.value = '1LA';
                    idState1D.value = '1LD';
                    idState0E.value = '1RH';
                    idState1E.value = '0LA';
                    break;
                case '6':
                    idState0A.value = '1RB';
                    idState1A.value = '0LD';
                    idState0B.value = '1RC';
                    idState1B.value = '0RF';
                    idState0C.value = '1LC';
                    idState1C.value = '1LA';
                    idState0D.value = '0LE';
                    idState1D.value = '1RH';
                    idState0E.value = '1LF';
                    idState1E.value = '0RB';
                    idState0F.value = '0RC';
                    idState1F.value = '0RE';
                    break;
                case '6BB':
                    idState0A.value = '1RB';
                    idState1A.value = '1RA';
                    idState0B.value = '1RC';
                    idState1B.value = '1RH';
                    idState0C.value = '1LD';
                    idState1C.value = '0RF';
                    idState0D.value = '1RA';
                    idState1D.value = '0LE';
                    idState0E.value = '0LD';
                    idState1E.value = '1RC';
                    idState0F.value = '1RA';
                    idState1F.value = '0RE';
                    break;
            }
        })
    })
}

function textareaSize(checkbox) {
    if (checkbox.checked) {
        var viewportWidth = document.documentElement.clientWidth - 20;
        var viewportHeight = document.documentElement.clientHeight - 200;
        idTapeDisplay.style.width = viewportWidth + 'px';
        idTapeDisplay.style.height = viewportHeight + 'px';
    }
    else {
        idTapeDisplay.style.width = null;
        idTapeDisplay.style.height = null;
        let cssStyle = document.defaultView.getComputedStyle(idTapeDisplay, null);
        idTapeDisplay.style.width = cssStyle.getPropertyValue('width');
        idTapeDisplay.style.height = cssStyle.getPropertyValue('height');
    }
}

function ruleChanged(rulestr) {
    var rule = rulestr.value;
    if (!(rule[0] == '0' || rule[0] == '1') ||
        !(rule[1] == 'L' || rule[1] == 'R') ||
        !(rule[2] == 'A' || rule[2] == 'B' || rule[2] == 'C' || rule[2] == 'D' || rule[2] == 'E' || rule[2] == 'F' || rule[2] == 'H'))
        {
            rulestr.value = '';
        }
}

const importObject = {
};

var waobj;
var waTMruleArr;
var waTMruleArrDV;
var waTMruleScrArr;
var waTMruleScrArrDV;
var waTMruleBidirArr;
var waTMruleBidirArrDV;
var waTMruleStateArr;
var waTMruleStateDV;
var waTMreturned;
var waPOS;

WebAssembly.instantiateStreaming(
    fetch("bb.wasm"),
    importObject
).then((obj) => {
    // Get exported memory
    waobj = obj;
    waTMruleArr = obj.instance.exports.waTMruleArr;
    waTMruleArrDV = new DataView(waTMruleArr.buffer);
    waTMruleScrArr = obj.instance.exports.waTMruleScrArr;
    waTMruleScrArrDV = new DataView(waTMruleScrArr.buffer);
    waTMruleBidirArr = obj.instance.exports.waTMruleBidirArr;
    waTMruleBidirArrDV = new DataView(waTMruleBidirArr.buffer);
    waTMruleStateArr = obj.instance.exports.waTMruleStateArr;
    waTMruleStateArrDV = new DataView(waTMruleStateArr.buffer);
});
function runWasmTM() {
    idStartWasmTM.disabled = true;

    let jsTMrulesString = "";
    let jsTMruleArr = new Int32Array(48);
    var jsTMruleScrArr = new Int32Array(48);
    var jsTMruleBidirArr = new Int32Array(48);
    var jsTMruleStateArr = new Int32Array(48);
    let domRuleArr = [idState0A, idState1A, idState0B, idState1B, idState0C, idState1C, idState0D, idState1D, idState0E, idState1E, idState0F, idState1F];
    for (let i = 0; i < 12; ++i) {
        let rule = domRuleArr[i].value;
        jsTMruleArr[i] = rule[0] * 0x10000 + ((rule[1] == 'R') ? 0 : 1) * 0x100 + (rule.charCodeAt(2) - 0x41);
        jsTMruleScrArr[i] = rule[0];
        jsTMruleBidirArr[i] = ((rule[1] == 'R') ? 1 : -1);
        jsTMruleStateArr[i] = (rule.charCodeAt(2) - 0x41);
        jsTMrulesString += rule;
    }
    const fetchTMrulesResponse = fetch('/?tmrules=' + jsTMrulesString);
    for (let i = 0; i < 12; ++i) {
        waTMruleArrDV.setInt32(4 * i, jsTMruleArr[i], true);
        waTMruleScrArrDV.setInt32(4 * i, jsTMruleScrArr[i], true);
        waTMruleBidirArrDV.setInt32(4 * i, jsTMruleBidirArr[i], true);
        waTMruleStateArrDV.setInt32(4 * i, jsTMruleStateArr[i], true);
    }
    const startTime = performance.now();
    waTMreturned = waobj.instance.exports.startWaTM();
    const endTime = performance.now();
    idPerf.value = endTime - startTime;
    let shifts_i64 = waTMreturned >> 32n;
    idShifts.innerHTML = shifts_i64.toString();
    waPOS = Number(BigInt.asUintN(17, waTMreturned)); // asUintN(log2(TAPE_LENGTH) + 1, ...) +1 for POS pointing outside tape memory
    // waobj.instance.exports.tapepos is a WA single-page memory chunk of 64K bytes. POS (bb.wat) is never negative
    const bytes = new Uint8Array(waobj.instance.exports.tapepos.buffer, 0, 65536); // Uint8Array(..., 0, TAPE_LENGTH)
    const nzbytes = [];
    if (waPOS < 32768) { // (waPOS < TAPE_LENGTH / 2)
        for (let i = 0; i <= waPOS; ++i) {
            if (bytes[i] != 0) nzbytes.push(bytes[i]);
        }
        nzbytes.push(0x2E);
        for (let i = waPOS + 1; i < 32768; ++i) {
            if (bytes[i] != 0) nzbytes.push(bytes[i]);
        }
        nzbytes.push(0x5E);
        for (let i = 32768; i < 65536; ++i) {
            if (bytes[i] != 0) nzbytes.push(bytes[i]);
        }
    }
    else {
        for (let i = 0; i < 32768; ++i) {
            if (bytes[i] != 0) nzbytes.push(bytes[i]);
        }
        nzbytes.push(0x5E);
        for (let i = 32768; i <= waPOS; ++i) {
            if (bytes[i] != 0) nzbytes.push(bytes[i]);
        }
        nzbytes.push(0x2E);
        for (let i = waPOS + 1; i < 65536; ++i) {
            if (bytes[i] != 0)
                nzbytes.push(bytes[i]);
        }
    }
    const nzbytesArr  = new Uint8Array(nzbytes);   
    idTapeDisplay.value= new TextDecoder("utf-8").decode(nzbytesArr);

    idStartWasmTM.disabled = false;
}