.data                                   
i:        .word     0                   
m:        .word     0                   
.text                                   
move      $fp       $sp                 
j         main                          
main:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       20        
sw        $ra       -8($fp)             
$end_main:                              
li        $v0       10                  
syscall                                 
