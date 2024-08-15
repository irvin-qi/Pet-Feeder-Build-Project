const dotenv = require('dotenv');
dotenv.config();

const cors = require('cors');
const express = require('express');
const admin = require('firebase-admin');
const serviceAccount = require('../firebase-admin-key.json');

// Initialize Firebase Admin SDK
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: 'https://theta-tau-pet-feeder-default-rtdb.firebaseio.com' // Replace with your database URL
});

// Creating express app
const app = express();

// Telling it to use cors for all routes
app.use(cors());
app.use(express.json());

// Test route
app.get('/', (req, res) => {
  res.send('What up dawg');
});

// Example route to get data from Firebase Realtime Database
app.get('/api/data', async (req, res) => {
  try {
    const db = admin.database();
    const ref = db.ref('/FireBaseTest.json'); // Replace with your data path
    const snapshot = await ref.once('value');
    const data = snapshot.val();
    res.json(data);
  } catch (error) {
    res.status(500).json({ error: 'Error fetching data' });
  }
});

// Route to handle GET request from Arduino
app.get('/api/arduino', async (req, res) => {
  console.log('Received request for /api/arduino with data:', req.query.data); // Log the request
  try {
    const db = admin.database();
    const ref = db.ref('path/to/your/data'); // Replace with your data path
    await ref.set({ FireBaseTest: req.query.data });
    res.status(200).json({ message: 'Data saved to Firebase' });
  } catch (error) {
    console.error('Error saving data to Firebase:', error); // Log errors
    res.status(500).json({ error: 'Error saving data' });
  }
});

// Route to handle POST request from the frontend to set FeedOn
app.post('/api/dispense-food', async (req, res) => {
  console.log('Setting FeedOn to true in Firebase');
  try {
    const db = admin.database();
    const feedRef = db.ref('FeedOn/value'); // Replace with your data path

    await feedRef.set(true);

    res.status(200).json({ message: 'FeedOn set to true in Firebase' });

  } catch (error) {
    console.error('Error setting FeedOn in Firebase:', error);
    res.status(500).json({ error: 'Error setting FeedOn' });
  }
});

// Route to handle POST request from the frontend to set WaterOn
app.post('/api/dispense-water', async (req, res) => {
  console.log('Setting WaterOn to true in Firebase');
  try {
    const db = admin.database();
    const waterRef = db.ref('WaterOn/value'); // Replace with your data path

    await waterRef.set(true);

    res.status(200).json({ message: 'WaterOn set to true in Firebase' });

  } catch (error) {
    console.error('Error setting WaterOn in Firebase:', error);
    res.status(500).json({ error: 'Error setting WaterOn' });
  }
});

// New endpoint to get current water level
app.get('/api/water-level', async (req, res) => {
  try {
    console.log('Fetching water level from Firebase');
    const db = admin.database();
    const waterRef = db.ref('WaterLevel/WaterLevel'); // Replace with your water level path

    const waterSnapshot = await waterRef.once('value');
    const waterLevel = waterSnapshot.val();

    res.status(200).json({ waterLevel });
  } catch (error) {
    console.error('Error fetching water level from Firebase:', error);
    res.status(500).json({ error: 'Error fetching water level' });
  }
});

// Start server
const PORT = process.env.DEVPORT || 3000;
app.listen(PORT, () => console.log(`Server started on PORT ${PORT}`));
