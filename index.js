let banana = false;
let i = 1;

let title = document.getElementById("timmy");
let newkid = document.getElementById("newkidname");
let newMoney = document.getElementById("newkidmoney");
let lil = document.getElementById("knownKids");
let Gelder = [40];

function neuesKind() {
    let cont = document.createElement("li");
    
    let nam = document.createElement("div");
    let mon = document.createElement("div");

    let add10btn = document.createElement("button");
    add10btn.onclick = addMoney.bind(null, i); // Bind the index correctly
    add10btn.innerHTML = "+10€";

    nam.className = "knownKidName";
    nam.innerHTML = newkid.value;
    mon.innerHTML = newMoney.value;
    mon.className = "knownKidMoney";

    cont.appendChild(nam);
    cont.appendChild(mon);
    cont.appendChild(add10btn);
    cont.className = "knownkidContainer";

    lil.appendChild(cont);

    // Store the initial money value in the Gelder array
    Gelder[i] = parseInt(newMoney.value, 10);
    newkid.value = "";
    i++;
}

function addMoney(wer, event) {
    event.preventDefault(); // Prevent the form from submitting

    // Update the money in the Gelder array
    Gelder[wer] += 10;

    // Find the corresponding money element and update its display
    let moneyElement = lil.children[wer].querySelector(".knownKidMoney");
    moneyElement.innerHTML = Gelder[wer];
}