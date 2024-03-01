
import './App.css';
import React, {useState, useEffect} from 'react';
// import logo from './kisspng-leaf-logo-brand-plant-stem-folha-5acb0798d686f9.0092563815232551928787.jpg'
import Axios from "axios";

// console.log(logo);

import { LineChart, Line, CartesianGrid, XAxis, YAxis, Tooltip } from 'recharts';
// const data = [{name: 'Page A', uv: 400, pv: 2400, amt: 2400},
// {name: 'Page B', uv: 500, pv: 2500, amt: 2400}];

// const renderLineChart = (
//   <LineChart width={600} height={300} data={data} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
//   <Line type="monotone" dataKey="uv" stroke="#8884d8" />
//   <CartesianGrid stroke="#ccc" strokeDasharray="5 5" />
//   <XAxis dataKey="name" />
//   <YAxis />
//   <Tooltip />
// </LineChart>
// );


function App() {
  
//   const [humidite, setHumidite] = useState("")
//   const [temperature, setTemperature] = useState("")
//   const [luminosite, setLuminosite] = useState("")
  const [plantLogList, setLogPlant] = useState([])

  useEffect(() => {
    Axios.get('http://87.89.194.28:4000/api/get').then((response) => {
        // console.log(response)
        // console.log(response.data)
        response.data.forEach(e => {
            e.date = e.date.replace("T", " ").replace(".000Z", "")
            e.humidite = e.humidite.replace("000", " ")
        });
        console.log(response.data)
        setLogPlant(response.data)
    });
  }, []);

//   const submitPlantValue = () => {
//     Axios.post('http://87.89.194.28:4000/api/insert',{
//     temperature: temperature,
//     humidite: humidite,
//     luminosite: luminosite,
//   }).then(() => {
//     alert("succesful insert");
//   });
//   };
  

const renderLineChart = (
  <LineChart width={550} height={300} data={plantLogList} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
  <Line type="monotone" dataKey="temperature" stroke="#FF595E" />
  <CartesianGrid stroke="#ccc" strokeDasharray="5 5" />
  <XAxis dataKey="date" tick={false}/>
  <YAxis />
  <Tooltip />
</LineChart>
);

const renderLineChart2 = (
  <LineChart width={550} height={300} data={plantLogList} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
  <Line type="monotone" dataKey="luminosite" stroke="#FFCA3A" />
  <CartesianGrid stroke="#ccc" strokeDasharray="5 5" />
  <XAxis dataKey="date" tick={false}/>
  <YAxis />
  <Tooltip />
</LineChart>
);

const renderLineChart3 = (
  <LineChart width={550} height={300} data={plantLogList} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
  <Line type="monotone" dataKey="humidite" stroke="#1982C4" />
  <CartesianGrid stroke="#ccc" strokeDasharray="5 5" />
  <XAxis dataKey="date" tick={false}/>
  <YAxis />
  <Tooltip />
</LineChart>
);




  return (
    <div className="App">
    <h1 style={{ color: '#59CD90' }}>Le_Kube 2.0</h1>
	<div className="form">
    {/* <label>temperature:</label>
  <input type="number" name="temperature" onChange={(e)=>
      setTemperature(e.target.value)} />
  <label>humidite:</label>
  <input type="number" name="humidite" onChange={(e)=>
      setHumidite(e.target.value)} />
  <label>luminosite:</label>
  <input type="number" name="luminosite" onChange={(e)=>
      setLuminosite(e.target.value)}/> */}
  {/* <button onClick={submitPlantValue}>Submit</button> */}
  {renderLineChart}
  <h3 style={{ color: '#FF595E' }}>Température</h3>
  {renderLineChart2}
  <h3 style={{ color: '#FFCA3A' }}>Luminosité</h3>
  {renderLineChart3}
  <h3 style={{ color: '#1982C4' }}>Humidité</h3>
  </div>
    </div>
  );
}

export default App;
