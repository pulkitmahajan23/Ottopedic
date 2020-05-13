const express = require('express');
const router = express.Router();

 
//welcome page
router.get('/', function(req,res){
    res.render('quiz');
});

router.post('/', function(req,res){
    res.json({status:"OK"});
});



module.exports = router;
