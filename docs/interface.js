document.addEventListener("DOMContentLoaded", function () {    
    document.querySelector("#loadGame").onclick = async (event) => {
        const fileSelect = document.getElementById('selected-file');
        fileSelect.click();
    }

});

const loadROM = (input) => {
    let file = input.files[0];
    const reader = new FileReader();
    reader.readAsArrayBuffer(file);

    reader.onloadend = () => {
        const data = reader.result; 
        const gameData = new Uint8Array(data);
        Module.ccall('LoadROM', null, ['number', 'array'], [gameData.length, gameData]);
    }
}

const Patterntables = {
    patterntable1Canvas: document.getElementById('canvas-patterntable1'),
    patterntable2Canvas: document.getElementById('canvas-patterntable2'),
    patterntable1: null,
    patterntable2: null,
    update() {
        const pattertable1Ctx = this.patterntable1Canvas.getContext('2d');
        const patterntable1 = new ImageData(this.patterntable1, 128, 128);
        pattertable1Ctx.putImageData(patterntable1, 0, 0);

        const pattertable2Ctx = this.patterntable2Canvas.getContext('2d');
        const patterntable2 = new ImageData(this.patterntable2, 128, 128);
        pattertable2Ctx.putImageData(patterntable2, 0, 0);
    }
}

const Interface = {
    registers: null,
    registersElement: document.getElementById('registers'),
    updateScreen() {
        const detail = `Accumulator: ${this.registers[0].toString(16)} X: ${this.registers[1].toString(16)} Y: ${this.registers[2].toString(16)} SP: ${this.registers[3].toString(16)} SR: ${("0000000" + this.registers[4].toString(2)).substr(-8)}`;
        this.registersElement.innerText = detail;
    }
}