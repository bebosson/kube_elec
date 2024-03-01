const moment = require('moment')
const express = require('express')
const bodyParser = require('body-parser')
const app = express()
const mysql = require('mysql')
const cors = require('cors')
const db = mysql.createPool({
  host: '127.0.0.1',
  user: 'bebosson',
  password :'mysqlpassword',
  database : 'kube',
  socketPath : '/var/run/mysqld/mysqld.sock',
  // insecureAuth: true
});



app.use(express.json())
app.use(cors());
app.use(bodyParser.urlencoded({extended:true}))


app.get('/api/get' , (req, res) => {
  const sqlSelect = 
  "SELECT * FROM plant"
  db.query(sqlSelect, (err, result) =>{
    res.send(result);
    console.log(result);
  });
  console.log('requete get recues');

})




app.post("/api/insert", (req, res) => {
  // const date = new Date().toISOString().slice(0, 19).replace('T', ' ');
  // console.log('debut /api/insert')
  console.log(req.body) 
  const date = moment().format('YYYY-MM-D h:mm:ss');
  const temperature = req.body.temperature;
  const humidite = req.body.humidite;
  const luminosite = req.body.luminosite;
  console.log(temperature, humidite, luminosite)
  const sqlInsert = "INSERT INTO plant (date, temperature, humidite, luminosite) VALUES (?,?,?,?)";
  db.query(sqlInsert, [date,temperature, humidite, luminosite], (err, result) =>{
     console.log(result);
     console.log(date);
    // console.log(temperature);
    // console.log(humidite);
    // console.log(luminosite);
    //  console.log(err);
  });
});

try {
app.listen(4000, () => {
  console.log("running on port 4000");
})} 
catch(e) {
  console.log(e)
};
