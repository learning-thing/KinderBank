let banana = false;
let i = 0;
let namen = [];

let title = document.getElementById("timmy");
let newkid = document.getElementById("newkidname");
let newMoney = document.getElementById("newkidmoney");
let lil = document.getElementById("knownKids");
let Gelder = [];

function load() {
    i = parseInt(localStorage.getItem("i"));
    Gelder = localStorage.getItem("Gelder").split(",");
    namen = localStorage.getItem("namen").split(",");

    for (let x = 0; x<i; x+=1) {
        addkid(namen[x], Gelder[x], x);
    }
}

function neuesKind() {
    let cont = document.createElement("li");
    
    let nam = document.createElement("div");
    let mon = document.createElement("div");

    let add10btn = document.createElement("button");
    add10btn.onclick = addMoney.bind(null, i); // Bind the index correctly
    add10btn.innerHTML = "+1€";

    if (newkid.value=="") {
        return 0;
    }

    nam.className = "knownKidName";
    nam.innerHTML = newkid.value;
    namen[i] = newkid.value;
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

function addkid(name, money, id) {
    let cont = document.createElement("li");
    let nam = document.createElement("div");
    let mon = document.createElement("div");
    let add10btn = document.createElement("button");

    add10btn.onclick = addMoney.bind(null, id); // Bind the index correctly
    add10btn.innerHTML = "+1€";

    nam.className = "knownKidName";
    nam.innerHTML = name;
    namen[i] = name;
    mon.innerHTML = money;
    mon.className = "knownKidMoney";

    cont.appendChild(nam);
    cont.appendChild(mon);
    cont.appendChild(add10btn);
    cont.className = "knownkidContainer";

    lil.appendChild(cont);

    // Store the initial money value in the Gelder array
    Gelder[i] = parseInt(money, 10);
    newkid.value = "";
}

function addMoney(wer, event) {
    event.preventDefault(); // Prevent the form from submitting

    // Update the money in the Gelder array
    Gelder[wer] = parseInt(Gelder[wer])+1;

    // Find the corresponding money element and update its display
    let moneyElement = lil.children[wer].querySelector(".knownKidMoney");
    moneyElement.innerHTML = Gelder[wer];
}

function stealMoney(wer, event) {
    event.preventDefault(); // Prevent the form from submitting

    // Update the money in the Gelder array
    Gelder[wer] = parseInt(Gelder[wer])+1;

    // Find the corresponding money element and update its display
    let moneyElement = lil.children[wer].querySelector(".knownKidMoney");
    moneyElement.innerHTML = Gelder[wer];
}

function save() {
    localStorage.setItem("i", i);
    localStorage.setItem("Gelder", Gelder);
    localStorage.setItem("namen", namen);
}

function empty() {
    localStorage.clear();
}