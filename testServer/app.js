const express = require('express')
const app = express()
const port = 3000

app.use(express.json());

app.get('/', (req, res) => {
  res.send('Hello World!')
})

app.listen(port, '192.168.88.105', () => {
  console.log(`listening on port ${port}`)
})

app.post("/v1/soil/post", (req, res) => {
    console.log(req.body)
    res.send(`recive\n${JSON.stringify(req.body)}`)
})