export function App(props) {

  return `
  <div class="flex space-between">
    <h1 contenteditable="true">${props.city}</h1>
    <span id="temp" class="light-text">${props.currentTemp}&deg;</span>
  </div>

  <div class="flex">
    <div class="light-text">
        <div>
            <img src="./humidity.svg" alt="">
            ${props.currentHumidity}%
        </div>
        <div>
            <img src="./wind-turbine.svg" alt="">
            ${props.currentWind}km/h
        </div>
        <div>
            <img src="./gauge.svg" alt="">
            ${props.currentPressure}hPa
        </div>
    </div>
    <img class="ma" height="150" src="http://openweathermap.org/img/wn/${props.currentImg}@2x.png" alt="">
  </div>
  <div class="flex center">
    ${WeatherList(props.weekly)}
  </div>
  
  `
}

export function WeatherList(props) {
  return props.map(item =>`
  <div class="card flex column">
    <img class="mb40" height="50" src="http://openweathermap.org/img/wn/${item.icon}@2x.png" alt="">
    <div>${item.day}</div>
    <div>${item.temp}&deg;</div>
  </div>
  `).join("")
}

