.data                                   
d:        .space    40                  
str_0:    .asciiz   "a:"                
str_1:    .asciiz   " b:"               
str_2:    .asciiz   "\n"                
str_3:    .asciiz   "big\n"             
str_4:    .asciiz   "less\n"            
str_5:    .asciiz   "big or equal\n"          
str_6:    .asciiz   "less or equal\n"          
str_7:    .asciiz   "not equal\n"          
str_8:    .asciiz   "equal\n"           
str_9:    .asciiz   "a:"                
str_10:   .asciiz   "b:"                
str_11:   .asciiz   "a - b = "          
str_12:   .asciiz   "\n"                
str_13:   .asciiz   "a + b = "          
str_14:   .asciiz   "\n"                
str_15:   .asciiz   "a * b = "          
str_16:   .asciiz   "\n"                
str_17:   .asciiz   "a / b = "          
str_18:   .asciiz   "\n"                
str_19:   .asciiz   "\n"                
str_20:   .asciiz   "\n"                
str_21:   .asciiz   "do...while\n"          
str_22:   .asciiz   "\n"                
str_23:   .asciiz   "test_char: "          
str_24:   .asciiz   "\n"                
str_25:   .asciiz   "test_int: "          
str_26:   .asciiz   "\n"                
str_27:   .asciiz   "test_char_array: "          
str_28:   .asciiz   "\n"                
str_29:   .asciiz   "test_int_array: "          
str_30:   .asciiz   "\n"                
str_31:   .asciiz   "foo2: "            
str_32:   .asciiz   "fibonacci: "          
str_33:   .asciiz   "\n"                
.text                                   
move      $fp       $sp                 
j         main                          
foo:                                    
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       20        
sw        $ra       -8($fp)             
sw        $a0       -12($fp)            
sw        $a1       -16($fp)            
la        $a0       str_0               
li        $v0       4                   
syscall                                 
lw        $a0       -12($fp)            
li        $v0       1                   
syscall                                 
la        $a0       str_1               
li        $v0       4                   
syscall                                 
lw        $a0       -16($fp)            
li        $v0       1                   
syscall                                 
la        $a0       str_2               
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
ble       $t8       $t9       label_0   
la        $a0       str_3               
li        $v0       4                   
syscall                                 
label_0:                                
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
bge       $t8       $t9       label_2   
la        $a0       str_4               
li        $v0       4                   
syscall                                 
label_2:                                
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
blt       $t8       $t9       label_4   
la        $a0       str_5               
li        $v0       4                   
syscall                                 
label_4:                                
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
bgt       $t8       $t9       label_6   
la        $a0       str_6               
li        $v0       4                   
syscall                                 
label_6:                                
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
beq       $t8       $t9       label_8   
la        $a0       str_7               
li        $v0       4                   
syscall                                 
label_8:                                
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
bne       $t8       $t9       label_10  
la        $a0       str_8               
li        $v0       4                   
syscall                                 
label_10:                               
li        $v0       233                 
j         $end_foo                      
$end_foo:                               
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
foo4:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       24        
sw        $ra       -8($fp)             
la        $a0       str_9               
li        $v0       4                   
syscall                                 
li        $v0       5                   
syscall                                 
sw        $v0       -12($fp)            
la        $a0       str_10              
li        $v0       4                   
syscall                                 
li        $v0       5                   
syscall                                 
sw        $v0       -16($fp)            
la        $a0       str_11              
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_12              
li        $v0       4                   
syscall                                 
la        $a0       str_13              
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_14              
li        $v0       4                   
syscall                                 
la        $a0       str_15              
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
mul       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_16              
li        $v0       4                   
syscall                                 
la        $a0       str_17              
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
div       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_18              
li        $v0       4                   
syscall                                 
$end_foo4:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
test_char:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       24        
sw        $ra       -8($fp)             
li        $t8       97                  
and       $t8       $t8       0xFF      
sw        $t8       -12($fp)            
li        $t8       98                  
and       $t8       $t8       0xFF      
sw        $t8       -16($fp)            
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $v0       4($sp)              
j         $end_test_char                    
$end_test_char:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
test_int:                               
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       36        
sw        $ra       -8($fp)             
li        $t7       1                   
li        $t8       0                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
li        $t7       2                   
li        $t8       1                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
lw        $t8       -12($fp)            
sw        $t8       4($sp)              
lw        $t8       -16($fp)            
sw        $t8       12($sp)             
lw        $t8       12($sp)             
li        $t9       666                 
add       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $t8       4($sp)              
lw        $t9       16($sp)             
mul       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t7       8($sp)              
li        $t8       2                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
lw        $t8       -20($fp)            
sw        $t8       4($sp)              
lw        $v0       4($sp)              
j         $end_test_int                    
$end_test_int:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
test_char_array:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       32        
sw        $ra       -8($fp)             
li        $t7       97                  
li        $t8       0                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
and       $t7       $t7       0xFF      
sw        $t7       ($t9)               
li        $t7       98                  
li        $t8       1                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
and       $t7       $t7       0xFF      
sw        $t7       ($t9)               
lw        $t8       -12($fp)            
and       $t8       $t8       0xFF      
sw        $t8       4($sp)              
lw        $t8       -16($fp)            
and       $t8       $t8       0xFF      
sw        $t8       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t7       8($sp)              
li        $t8       2                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
and       $t7       $t7       0xFF      
sw        $t7       ($t9)               
lw        $t8       -20($fp)            
and       $t8       $t8       0xFF      
sw        $t8       4($sp)              
lw        $v0       4($sp)              
j         $end_test_char_array                    
$end_test_char_array:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
test_int_array:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       32        
sw        $ra       -8($fp)             
li        $t7       1                   
li        $t8       0                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
li        $t7       2                   
li        $t8       1                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
lw        $t8       -12($fp)            
sw        $t8       4($sp)              
lw        $t8       -16($fp)            
sw        $t8       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
mul       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t7       8($sp)              
li        $t8       2                   
add       $t9       $fp       -12       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
lw        $t8       -20($fp)            
sw        $t8       4($sp)              
lw        $v0       4($sp)              
j         $end_test_int_array                    
$end_test_int_array:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
foo2:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       104       
sw        $ra       -8($fp)             
sw        $a0       -12($fp)            
sw        $a1       -16($fp)            
li        $t8       123                 
sw        $t8       -20($fp)            
li        $t8       456                 
sw        $t8       -24($fp)            
li        $t8       789                 
sw        $t8       -28($fp)            
li        $t7       233                 
li        $t8       9                   
add       $t9       $fp       -32       
mul       $t8       $t8       4         
sub       $t9       $t9       $t8       
sw        $t7       ($t9)               
lw        $t8       -68($fp)            
sw        $t8       4($sp)              
lw        $t8       -16($fp)            
lw        $t9       -12($fp)            
sub       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
mul       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       -24($fp)            
lw        $t9       -28($fp)            
sub       $t9       $t8       $t9       
sw        $t9       24($sp)             
lw        $t8       -20($fp)            
lw        $t9       24($sp)             
div       $t9       $t8       $t9       
sw        $t9       20($sp)             
lw        $t8       20($sp)             
lw        $t9       -28($fp)            
mul       $t9       $t8       $t9       
sw        $t9       28($sp)             
lw        $t8       28($sp)             
lw        $t9       -20($fp)            
mul       $t9       $t8       $t9       
sw        $t9       32($sp)             
lw        $t8       8($sp)              
lw        $t9       32($sp)             
add       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $t8       16($sp)             
sw        $t8       -20($fp)            
lw        $v0       -20($fp)            
j         $end_foo2                     
$end_foo2:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
test_loop:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       44        
sw        $ra       -8($fp)             
li        $t8       0                   
sw        $t8       -12($fp)            
label_12:                               
lw        $t8       -12($fp)            
li        $t9       10                  
bge       $t8       $t9       label_15  
j         label_14                      
label_13:                               
lw        $t8       -12($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -12($fp)            
j         label_12                      
label_14:                               
li        $t8       0                   
sw        $t8       -16($fp)            
label_16:                               
lw        $t8       -16($fp)            
li        $t9       10                  
bge       $t8       $t9       label_19  
j         label_18                      
label_17:                               
lw        $t8       -16($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -16($fp)            
j         label_16                      
label_18:                               
li        $t8       0                   
sw        $t8       -20($fp)            
label_20:                               
lw        $t8       -20($fp)            
li        $t9       10                  
bge       $t8       $t9       label_23  
j         label_22                      
label_21:                               
lw        $t8       -20($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -20($fp)            
j         label_20                      
label_22:                               
lw        $t8       -12($fp)            
li        $t9       100                 
mul       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       -16($fp)            
li        $t9       10                  
mul       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
lw        $t9       -20($fp)            
add       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $t8       16($sp)             
sw        $t8       -24($fp)            
lw        $a0       -24($fp)            
li        $v0       1                   
syscall                                 
la        $a0       str_19              
li        $v0       4                   
syscall                                 
j         label_21                      
label_23:                               
j         label_17                      
label_19:                               
j         label_13                      
label_15:                               
li        $t8       0                   
sw        $t8       -12($fp)            
label_24:                               
lw        $t8       -12($fp)            
li        $t9       10                  
bge       $t8       $t9       label_27  
j         label_26                      
label_25:                               
lw        $t8       -12($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -12($fp)            
j         label_24                      
label_26:                               
li        $t8       0                   
sw        $t8       -16($fp)            
label_28:                               
lw        $t8       -16($fp)            
li        $t9       10                  
bge       $t8       $t9       label_31  
j         label_30                      
label_29:                               
lw        $t8       -16($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -16($fp)            
j         label_28                      
label_30:                               
li        $t8       0                   
sw        $t8       -20($fp)            
label_32:                               
lw        $t8       -20($fp)            
li        $t9       10                  
bge       $t8       $t9       label_35  
j         label_34                      
label_33:                               
lw        $t8       -20($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -20($fp)            
j         label_32                      
label_34:                               
lw        $t8       -12($fp)            
li        $t9       100                 
mul       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       -16($fp)            
li        $t9       10                  
mul       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
lw        $t9       -20($fp)            
add       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $a0       16($sp)             
li        $v0       1                   
syscall                                 
j         label_33                      
label_35:                               
j         label_29                      
label_31:                               
j         label_25                      
label_27:                               
label_36:                               
lw        $t8       -12($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -12($fp)            
li        $t8       10                  
sw        $t8       -16($fp)            
li        $t8       10                  
sw        $t8       -20($fp)            
label_37:                               
lw        $t8       -16($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -16($fp)            
li        $t8       10                  
sw        $t8       -20($fp)            
label_38:                               
lw        $t8       -20($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -20($fp)            
lw        $t8       -12($fp)            
li        $t9       100                 
mul       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       -16($fp)            
li        $t9       10                  
mul       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $t8       4($sp)              
lw        $t9       12($sp)             
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
lw        $t9       -20($fp)            
add       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $t8       16($sp)             
sw        $t8       -24($fp)            
lw        $a0       -24($fp)            
li        $v0       1                   
syscall                                 
la        $a0       str_20              
li        $v0       4                   
syscall                                 
lw        $t8       -20($fp)            
li        $t9       0                   
bgt       $t8       $t9       label_38  
lw        $t8       -16($fp)            
li        $t9       0                   
bgt       $t8       $t9       label_37  
lw        $t8       -12($fp)            
li        $t9       0                   
bgt       $t8       $t9       label_36  
label_39:                               
label_40:                               
label_41:                               
la        $a0       str_21              
li        $v0       4                   
syscall                                 
lw        $t8       -20($fp)            
li        $t9       0                   
bne       $t8       $t9       label_41  
lw        $t8       -16($fp)            
li        $t9       0                   
bne       $t8       $t9       label_40  
lw        $t8       -12($fp)            
li        $t9       0                   
bne       $t8       $t9       label_39  
$end_test_loop:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
quick_sort:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       48        
sw        $ra       -8($fp)             
sw        $a0       -12($fp)            
sw        $a1       -16($fp)            
lw        $t8       -12($fp)            
sw        $t8       -20($fp)            
lw        $t8       -16($fp)            
sw        $t8       -24($fp)            
lw        $t8       -12($fp)            
lw        $t9       -16($fp)            
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
li        $t9       2                   
div       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $t8       12($sp)             
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -28($fp)            
label_42:                               
lw        $t8       -24($fp)            
sw        $t8       -24($fp)            
label_43:                               
lw        $t8       -24($fp)            
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $t8       -28($fp)            
lw        $t9       4($sp)              
bge       $t8       $t9       label_46  
j         label_45                      
label_44:                               
lw        $t8       -24($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
sw        $t8       -24($fp)            
j         label_43                      
label_45:                               
j         label_44                      
label_46:                               
lw        $t8       -20($fp)            
sw        $t8       -20($fp)            
label_47:                               
lw        $t8       -20($fp)            
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $t8       -28($fp)            
lw        $t9       4($sp)              
ble       $t8       $t9       label_50  
j         label_49                      
label_48:                               
lw        $t8       -20($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       8($sp)              
lw        $t8       8($sp)              
sw        $t8       -20($fp)            
j         label_47                      
label_49:                               
j         label_48                      
label_50:                               
lw        $t8       -20($fp)            
lw        $t9       -24($fp)            
bgt       $t8       $t9       label_51  
lw        $t8       -20($fp)            
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -32($fp)            
lw        $t8       -24($fp)            
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $t7       4($sp)              
lw        $t8       -20($fp)            
mul       $t8       $t8       4         
sw        $t7       d($t8)              
lw        $t7       -32($fp)            
lw        $t8       -24($fp)            
mul       $t8       $t8       4         
sw        $t7       d($t8)              
lw        $t8       -20($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -20($fp)            
lw        $t8       -24($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -24($fp)            
label_51:                               
lw        $t8       -20($fp)            
lw        $t9       -24($fp)            
blt       $t8       $t9       label_42  
lw        $t8       -20($fp)            
lw        $t9       -16($fp)            
bge       $t8       $t9       label_53  
lw        $a0       -20($fp)            
lw        $a1       -16($fp)            
jal       quick_sort                    
label_53:                               
lw        $t8       -12($fp)            
lw        $t9       -24($fp)            
bge       $t8       $t9       label_55  
lw        $a0       -12($fp)            
lw        $a1       -24($fp)            
jal       quick_sort                    
label_55:                               
$end_quick_sort:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
fibonacci:                              
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       36        
sw        $ra       -8($fp)             
sw        $a0       -12($fp)            
lw        $t8       -12($fp)            
li        $t9       0                   
bne       $t8       $t9       label_57  
li        $v0       1                   
j         $end_fibonacci                    
label_57:                               
lw        $t8       -12($fp)            
li        $t9       1                   
bne       $t8       $t9       label_59  
li        $v0       1                   
j         $end_fibonacci                    
label_59:                               
lw        $t8       -12($fp)            
li        $t9       2                   
bne       $t8       $t9       label_61  
li        $v0       2                   
j         $end_fibonacci                    
label_61:                               
lw        $t8       -12($fp)            
li        $t9       2                   
beq       $t8       $t9       label_63  
lw        $t8       -12($fp)            
li        $t9       1                   
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
jal       fibonacci                     
sw        $v0       8($sp)              
lw        $t8       -12($fp)            
li        $t9       2                   
sub       $t9       $t8       $t9       
sw        $t9       16($sp)             
lw        $a0       16($sp)             
jal       fibonacci                     
sw        $v0       20($sp)             
lw        $t8       8($sp)              
lw        $t9       20($sp)             
add       $t9       $t8       $t9       
sw        $t9       12($sp)             
lw        $v0       12($sp)             
j         $end_fibonacci                    
label_63:                               
$end_fibonacci:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
foo5:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       24        
sw        $ra       -8($fp)             
sw        $a0       -12($fp)            
lw        $a0       -12($fp)            
li        $v0       1                   
syscall                                 
la        $a0       str_22              
li        $v0       4                   
syscall                                 
lw        $t8       -12($fp)            
li        $t9       10                  
bne       $t8       $t9       label_65  
lw        $v0       -12($fp)            
j         $end_foo5                     
label_65:                               
lw        $t8       -12($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $a0       4($sp)              
jal       foo5                          
sw        $v0       8($sp)              
lw        $v0       8($sp)              
j         $end_foo5                     
$end_foo5:                              
lw        $ra       -8($fp)             
move      $sp       $fp                 
lw        $fp       -4($fp)             
jr        $ra                           
main:                                   
sw        $fp       -4($sp)             
move      $fp       $sp                 
subu      $sp       $sp       24        
sw        $ra       -8($fp)             
li        $v0       5                   
syscall                                 
sw        $v0       -12($fp)            
lw        $t8       -12($fp)            
li        $t9       1                   
bne       $t8       $t9       label_67  
li        $a0       2                   
li        $a1       3                   
jal       foo                           
sw        $v0       4($sp)              
jal       foo4                          
label_67:                               
lw        $t8       -12($fp)            
li        $t9       2                   
bne       $t8       $t9       label_69  
la        $a0       str_23              
li        $v0       4                   
syscall                                 
jal       test_char                     
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       11                  
syscall                                 
la        $a0       str_24              
li        $v0       4                   
syscall                                 
la        $a0       str_25              
li        $v0       4                   
syscall                                 
jal       test_int                      
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_26              
li        $v0       4                   
syscall                                 
la        $a0       str_27              
li        $v0       4                   
syscall                                 
jal       test_char_array                    
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       11                  
syscall                                 
la        $a0       str_28              
li        $v0       4                   
syscall                                 
la        $a0       str_29              
li        $v0       4                   
syscall                                 
jal       test_int_array                    
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_30              
li        $v0       4                   
syscall                                 
la        $a0       str_31              
li        $v0       4                   
syscall                                 
li        $a0       2                   
li        $a1       3                   
jal       foo2                          
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
label_69:                               
lw        $t8       -12($fp)            
li        $t9       3                   
bne       $t8       $t9       label_71  
jal       test_loop                     
label_71:                               
lw        $t8       -12($fp)            
li        $t9       4                   
bne       $t8       $t9       label_73  
li        $t8       0                   
sw        $t8       -16($fp)            
label_75:                               
lw        $t8       -16($fp)            
li        $t9       10                  
bge       $t8       $t9       label_78  
j         label_77                      
label_76:                               
lw        $t8       -16($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -16($fp)            
j         label_75                      
label_77:                               
li        $t8       10                  
lw        $t9       -16($fp)            
sub       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t7       4($sp)              
lw        $t8       -16($fp)            
mul       $t8       $t8       4         
sw        $t7       d($t8)              
j         label_76                      
label_78:                               
li        $a0       0                   
li        $a1       10                  
jal       quick_sort                    
li        $t8       0                   
sw        $t8       -16($fp)            
label_79:                               
lw        $t8       -16($fp)            
li        $t9       10                  
bge       $t8       $t9       label_82  
j         label_81                      
label_80:                               
lw        $t8       -16($fp)            
li        $t9       1                   
add       $t9       $t8       $t9       
sw        $t9       4($sp)              
lw        $t8       4($sp)              
sw        $t8       -16($fp)            
j         label_79                      
label_81:                               
lw        $t8       -16($fp)            
mul       $t8       $t8       4         
lw        $t9       d($t8)              
sw        $t9       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
j         label_80                      
label_82:                               
label_73:                               
lw        $t8       -12($fp)            
li        $t9       5                   
bne       $t8       $t9       label_83  
la        $a0       str_32              
li        $v0       4                   
syscall                                 
li        $a0       10                  
jal       fibonacci                     
sw        $v0       4($sp)              
lw        $a0       4($sp)              
li        $v0       1                   
syscall                                 
la        $a0       str_33              
li        $v0       4                   
syscall                                 
li        $a0       0                   
jal       foo5                          
sw        $v0       4($sp)              
label_83:                               
$end_main:                              
li        $v0       10                  
syscall                                 
