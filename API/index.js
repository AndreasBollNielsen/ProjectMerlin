const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const path = require('path');

let states = [];
let speeds = [];

const app = express();
const port = 3000;

app.use(cors());

app.use(bodyParser.urlencoded({ extended: false}));
app.use(bodyParser.json());


// ENDPOINTS

// GET \\
app.get('/api/states', (req, res) => {
    res.send(states);
});

app.get('/api/speeds', (req, res) => {
    res.send(states);
});

// POST \\
app.post('/api/states/:stateKey', (req, res) => {
    const stateKey = req.params.stateKey;
    const stateIndex = states.findIndex(x => x.key == stateKey);

    if (states.find(x => x.key == stateKey)) {
        res.send(`Yup, its here, and it's currently ${states[stateIndex].state ? "on" : "off"}, changing state now to ${states[stateIndex].state ? "off" : "on"}`);
        states[stateIndex].state = states[stateIndex].state ? false : true;

    } else {
        res.status(404).send('Nope, not found, adding it now');
        states.push({key: stateKey, state: false});
    }
});

app.post('/api/speeds', (req, res) => {
    console.log(req.body);
    res.send(req.body.speed);
});

// DELETE \\




// END OF CODE
app.listen(port, () => {
    console.log(`API is now running on port ${port}!`);
});