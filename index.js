const express = require('express');
const mysql = require('mysql');

const app = express();
app.use(express.json());

const db = mysql.createConnection({
  host: '127.0.0.1',
  user: 'root',
  password: '123',
  database: 'tietokanta'
});

db.connect((err) => {
  if (err) throw err;
  console.log('MySQL connected...');
});

// CREATE (POST)
app.post('/books', (req, res) => {
  const book = req.body;
  const sql = 'INSERT INTO book SET ?';
  db.query(sql, book, (err, result) => {
    if (err) throw err;
    res.status(201).send('Book created');
  });
});

// READ (GET)
app.get('/books', (req, res) => {
  const sql = 'SELECT * FROM book';
  db.query(sql, (err, results) => {
    if (err) throw err;
    res.json(results);
  });
});

// UPDATE (PUT)
app.put('/books/:id', (req, res) => {
  const { id } = req.params;
  const book = req.body;
  const sql = 'UPDATE book SET ? WHERE id_book = ?';
  db.query(sql, [book, id], (err, result) => {
    if (err) throw err;
    res.send('Book updated');
  });
});

// DELETE (DELETE)
app.delete('/books/:id', (req, res) => {
  const { id } = req.params;
  const sql = 'DELETE FROM book WHERE id_book = ?';
  db.query(sql, [id], (err, result) => {
    if (err) throw err;
    res.send('Book deleted');
  });
});

app.listen(3000, () => {
  console.log('Server is running on port 3000');
});

app.put('/books/:id', (req, res) => {
    const id = req.params.id;
    const { name, author, isbn } = req.body;

    if (!name || !author || !isbn) {
        return res.status(400).send('All fields are required');
    }

    const sql = `UPDATE book SET name = ?, author = ?, isbn = ? WHERE id_book = ?`;
    connection.query(sql, [name, author, isbn, id], (err, result) => {
        if (err) {
            return res.status(500).send('Server error');
        }

        if (result.affectedRows === 0) {
            return res.status(404).send('Book not found');
        }

        res.send('Book updated');
    });
});



app.get('/car', (req, res) => {
    const sql = 'SELECT * FROM car';
    db.query(sql, (err, results) => {
      if (err) throw err;
      res.json(results);
    });
  });

  app.post('/car', (req, res) => {
    const { branch, model } = req.body;
    if (!branch || !model) {
        return res.status(400).send('All fields are required');
    }
    const sql = 'INSERT INTO car SET ?';
    db.query(sql, req.body, (err, result) => {
        if (err) {
            return res.status(500).send('Server error');
        }
        res.send('Car created');
    });
});

app.get('/car/:id', (req, res) => {
    const id = req.params.id;
    const sql = 'SELECT * FROM car WHERE id = ?';
    db.query(sql, [id], (err, result) => {
        if (err) {
            return res.status(500).send('Server error');
        }
        if (result.length === 0) {
            return res.status(404).send('Car not found');
        }
        res.json(result[0]);
    });
});

app.put('/car/:id', (req, res) => {
    const id = req.params.id;
    const { branch, model } = req.body;

    if (!branch || !model) {
        return res.status(400).send('All fields are required');
    }

    const sql = 'UPDATE car SET branch = ?, model = ? WHERE id = ?';
    db.query(sql, [branch, model, id], (err, result) => {
        if (err) {
            return res.status(500).send('Server error');
        }

        if (result.affectedRows === 0) {
            return res.status(404).send('Car not found');
        }

        res.send('Car updated');
    });
});

app.delete('/car/:id', (req, res) => {
    const id = req.params.id;
    const sql = 'DELETE FROM car WHERE id = ?';
    db.query(sql, [id], (err, result) => {
        if (err) {
            return res.status(500).send('Server error');
        }

        if (result.affectedRows === 0) {
            return res.status(404).send('Car not found');
        }

        res.send('Car deleted');
    });
});
