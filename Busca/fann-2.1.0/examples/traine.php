<?php
if ( ($ann = fann_create("mushroom_float.net")) == FALSE )
  exit("Could not create ANN.");
if ( fann_train($ann, "scaling.data", 100000, 0.00001) == FALSE )
  exit("Could not train ANN.");

if ( ($output = fann_run($ann, array(0, 1))) == FALSE )
  exit("Could not run ANN.");
else
  print_r($output);
?>

