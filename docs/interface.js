document.addEventListener("DOMContentLoaded", function () {
    document.querySelector("#loadGame").onclick = async (event) => {
        const data = [12, 6, 0]; 
        const gameData = new Uint8Array(data);
        Module.ccall('LoadROM', null, ['number', 'array'], [3, gameData]);
    }

});