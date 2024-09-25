// noinspection JSUnresolvedReference

import '../lib/std.js';

const sysLinks = [
    'about',
    'kitty',
    'error',
    'welcome',
    'settings'
];

$(document).ready(function () {
    $('#searchButton').on('click', () => {
        const query = $('#searchBar').val().trim();
        if (query) {
            if (query.startsWith('purr://')) {
                parseSysLink(query);
            } else if (query.includes('.')) {
                window.location.href = query.startsWith('http://') || query.startsWith('https://') ? query : 'https://' + query;
            } else {
                window.location.href = 'https://www.google.com/search?q=' + encodeURIComponent(query);
            }
        }
    });

    $('#searchBar').on('keydown', (event) => {
        if (event.key === 'Enter') $('#searchButton').click();
    });

    function parseSysLink(link) {
        const sysLink = link.replace('purr://', '');
        if (sysLinks.includes(sysLink)) window.location.href = `${sysLink}.html`;
    }
});

$(document).ready(() => {
    const $body = $('body');
    const $city = $('#city');
    const $temperature = $('#temperature');
    const $weatherIcon = $('#weather-icon');
    const $weatherDescription = $('#weather-description');
    const $weatherContainer = $('.weather-container');

    setRandomBackground();
    fetchWeather();

    function setRandomBackground() {
        const images = ['1.png', '2.png', '3.png', '4.png', '5.png', '6.png', '7.png', '8.png', '9.png'];
        const randomIndex = Math.floor(Math.random() * images.length);
        const randomImageUrl = `images/${images[randomIndex]}`;

        $body.css({
            'background-image': `url(${randomImageUrl})`,
            'background-size': 'cover',
            'background-position': 'center',
            'background-repeat': 'no-repeat',
        });
    }

    function fetchWeather() {
        $.getJSON('https://ipapi.co/json', (locationData) => {
            const city = locationData.city;

            $.getJSON(`https://api.open-meteo.com/v1/forecast?latitude=${locationData.latitude}&longitude=${locationData.longitude}&current_weather=true`, function (weatherData) {
                const temperature = Math.round((weatherData.current_weather.temperature * 1.8) + 32);
                const weatherCode = weatherData.current_weather.weathercode;
                const {description, iconUrl} = getWeatherConditionDescription(weatherCode);

                $city.text(city);
                $temperature.text(`${temperature}°F`);
                $weatherIcon.attr('src', iconUrl);
                $weatherDescription.text(description);

                $weatherContainer.css({'display': 'flex'})
                $weatherDescription.show();
            }).fail(() => console.error('Error fetching weather data'));
        }).fail(() => console.error('Error fetching location data'));
    }

    function getWeatherConditionDescription(weatherCode) {
        const weatherConditions = {
            "0": {description: "Clear", iconUrl: "https://openweathermap.org/img/wn/01d@2x.png"},
            "1": {description: "Mainly clear", iconUrl: "https://openweathermap.org/img/wn/02d@2x.png"},
            "2": {description: "Partly cloudy", iconUrl: "https://openweathermap.org/img/wn/03d@2x.png"},
            "3": {description: "Overcast", iconUrl: "https://openweathermap.org/img/wn/04d@2x.png"},
            "01": {description: "Clear sky tonight", iconUrl: "https://openweathermap.org/img/wn/01n@2x.png"},
            "02": {description: "Mainly clear tonight", iconUrl: "https://openweathermap.org/img/wn/02n@2x.png"},
            "03": {description: "Partly cloudy tonight", iconUrl: "https://openweathermap.org/img/wn/03n@2x.png"},
            "04": {description: "Overcast tonight", iconUrl: "https://openweathermap.org/img/wn/04n@2x.png"},
            "50": {description: "Unknown weather condition", iconUrl: ""},
            "51": {description: "Light drizzle", iconUrl: "https://openweathermap.org/img/wn/09d@2x.png"},
            "95": {description: "Thunderstorms", iconUrl: "https://openweathermap.org/img/wn/11d@2x.png"},
            "96": {description: "Thunderstorms with hail", iconUrl: "https://openweathermap.org/img/wn/11d@2x.png"},
            "99": {
                description: "Thunderstorms with heavy hail",
                iconUrl: "https://openweathermap.org/img/wn/11d@2x.png"
            },
            _: {description: "Unknown weather condition", iconUrl: ""}
        };

        return weatherConditions[weatherCode] || weatherConditions["_"];
    }
});