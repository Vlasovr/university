export const render = (component, target, props) => {
    target.innerHTML = component(props);
}


export const getFilteredData = (incomeData) => {
    const currentTemp = incomeData.list[0].main.temp.toFixed();
    const currentImg = incomeData.list[0].weather[0].icon;
    const currentWind = incomeData.list[0].wind.speed;
    const currentHumidity = incomeData.list[0].main.humidity;
    const currentPressure = incomeData.list[0].main.pressure;
    const weekly = [
        {
            day: new Date(incomeData.list[0].dt*1000).toDateString().split(" ")[0].toUpperCase(),
            icon: incomeData.list[0].weather[0].icon,
            temp: incomeData.list[0].main.temp.toFixed()
        },
        {
            day: new Date(incomeData.list[8].dt*1000).toDateString().split(" ")[0].toUpperCase(),
            icon: incomeData.list[8].weather[0].icon,
            temp: incomeData.list[8].main.temp.toFixed()
        },
        {
            day: new Date(incomeData.list[16].dt*1000).toDateString().split(" ")[0].toUpperCase(),
            icon: incomeData.list[16].weather[0].icon,
            temp: incomeData.list[16].main.temp.toFixed()
        },
        {
            day: new Date(incomeData.list[24].dt*1000).toDateString().split(" ")[0].toUpperCase(),
            icon: incomeData.list[24].weather[0].icon,
            temp: incomeData.list[24].main.temp.toFixed()
        },
        {
            day: new Date(incomeData.list[32].dt*1000).toDateString().split(" ")[0].toUpperCase(),
            icon: incomeData.list[32].weather[0].icon,
            temp: incomeData.list[32].main.temp.toFixed()
        },
    ];
    console.log(incomeData)
    return  {
        city: incomeData.city.name,
        currentWind,
        currentHumidity,
        currentPressure,
        currentTemp,
        currentImg,
        weekly,
        allWeather: incomeData.list
    }
}