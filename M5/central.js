// based on the example on https://www.npmjs.com/package/@abandonware/noble

const noble = require('@abandonware/noble');
var cors = require('cors')


const uuid_value = "2139";

let sensorValue = NaN;
let sensorValue2 = NaN;

let count = 0;

noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
         await noble.startScanningAsync(["1339"], false);
    }
});

noble.on('discover', async (peripheral) => {

    await noble.stopScanningAsync();
    if (peripheral.connectable) {
        console.log(peripheral.uuid);
        await peripheral.connectAsync();
        const {
            characteristics
        } = await peripheral.discoverAllServicesAndCharacteristicsAsync(); // ["1339", "1125"], [uuid_value])

        if (peripheral.uuid == "b87960518d0f697c8bad793d8538aa4b") {
            readData2(characteristics[0]);
            count++;
        }
    
        if (peripheral.uuid == "0678b4148ccedda1d6ea2190392ad4db") {
            readData(characteristics[0]);
            count++;
        }
        console.log(count);
    }
    if (count < 2)
        await noble.startScanningAsync("1125");

});


//
// read data periodically
//

// 1339
let readData = async (characteristic) => {
    const value = (await characteristic.readAsync());
    sensorValue = value.readFloatLE(0);
   // console.log(characteristic._serviceUuid);
    
    console.log("1339 data: " + sensorValue);

    // read data again in t milliseconds
    setTimeout(() => {
        readData(characteristic)
    }, 10);
}

// 1125
let readData2 = async (characteristic) => {
    const value = (await characteristic.readAsync());
    sensorValue2 = value.readFloatLE(0);
    
   console.log("1125 data: " + sensorValue2);

    // read data again in t milliseconds
    setTimeout(() => {
        readData2(characteristic)
    }, 10);
}



//
// hosting a web-based front-end and respond requests with sensor data
// based on example code on https://expressjs.com/
//
const express = require('express')
const app = express()
app.use(cors())
const port = 3000

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('index')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        sensorValue: sensorValue,
        sensorValue2: sensorValue2
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})
