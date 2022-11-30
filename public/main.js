let value=document.querySelector(".range")
let rangevalue=document.querySelector("#rangeValue")
console.log(rangevalue)
value.oninput =  e => {
    rangevalue.innerHTML=value.value
}
