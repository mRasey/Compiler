.data                                   
j:        .space    40                  
.text                                   
move      $fp       $sp                 
j         main                          
foo:                                    
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       12        
sw        $ra       -8($fp)             
li        $v0       97                  
j         $end_foo                      
$end_foo:                               
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
main:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       20        
sw        $ra       -8($fp)             
li        $t7       97                  
li        $t8       0                   
mul       $t8       $t8       4         
and       $t7       $t7       0xFF      
sw        $t7       j($t8)              
li        $t8       0                   
lw        $t9       j($t8)              
and       $t9       $t9       0xFF      
sw        $t9       4($sp)              
lw        $t8       4($sp)              
li        $t9       0                   
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $a0       8($sp)              
li        $v0       1                   
syscall                                 
$end_main:                              
li        $v0       10                  
syscall                                 
