import { App } from "./components.js";
import {  getFilteredData, render } from "./functions.js";

async function getWeather(city = "Minsk") {
    const response = await fetch(`https://api.openweathermap.org/data/2.5/forecast?q=${city}&appid=c026b7e260ea6a4cea507080c2ca5e3f&units=metric`);
    const data = await response.json();
    console.log (data);
    const filteredData = getFilteredData(data)

    render(
        App, 
        document.getElementById("root"),
        filteredData
    );
    
    document.querySelector("h1").addEventListener("keypress", event => {
        if(event.key !== "Enter") return;
        event.preventDefault()
        getWeather(event.target.innerHTML);
        event.target.innerHTML = null;
    });
}

getWeather();


