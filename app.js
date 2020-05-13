let app=require('express')();
let http=require('http').Server(app);
let io=require('socket.io')(http);
let fs=require('fs');
users=[];
connections=[];
const express = require('express');

const expressLayouts= require('express-ejs-layouts');
const mongoose = require('mongoose');
const flash= require('connect-flash');
const session = require('express-session');
const passport = require('passport');

const indexRouter = require('./routes/index');
const userRouter = require('./routes/user');
const quizRouter = require('./routes/quizscript');

//DB Config
const db= require('./config/keys').MongoURI;

//Passport Config
require('./config/passport')(passport);

//Connect to Mongo
mongoose.connect('mongodb://localhost/user',{ useNewUrlParser: true });
mongoose.connection.once('open', function(){
    console.log('Connection made!');
}).on('error',function(error){
    console.log('connection error', error);
})

io.on('connection',(socket)=>{
    connections.push(socket);
    console.log('connected: %s sockets connected',connections.length);
     socket.on('disconnect',()=>{
        connections.splice(connections.indexOf(socket),1)
        console.log('Disconnected:%s sockets connected',connections.length);
    })
    //send message
    socket.on('send message',function(data){
        // console.log(data);
        io.sockets.emit('new message',{msg:data})
    });
    socket.on('send image',function(image){
        io.sockets.emit('addimage','',image)

    })
});



//EJS
app.use(expressLayouts);
app.set('view engine','ejs');

//Bodyparser
app.use(express.urlencoded({extended:false}));

//Express Session
app.use(session({
    secret: 'secret',
    resave: true,
    saveUninitialized: true,
    
  }));

  //Passport middleware
  app.use(passport.initialize());
  app.use(passport.session());
  
  //Connect flash
  app.use(flash());

  //Global Vars
  app.use( (req, res, next) =>{
    res.locals.success_msg = req.flash('success_msg');
    res.locals.error_msg = req.flash('error_msg');
    res.locals.error = req.flash('error');
    next();
});

//Routes
app.use('/', indexRouter);
app.use('/user', userRouter);
app.use('/quiz', quizRouter);
app.use('/static', express.static('static'))
app.get('/user/msg',(req,res)=>{
    res.sendFile(__dirname + '/msg.html');
})



const PORT =process.env.PORT|| 8000;
app.listen(PORT,console.log(`Server started on port ${PORT}`));