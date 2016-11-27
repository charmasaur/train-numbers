<html>
  <body>
    <form>
      Number: <input type="text" name="number"><br>
      Allow exponentials: <input type="checkbox" name="exp"><br>
      <input type="submit" value="Go"><br>
    </form>
    <?php
      $val = $_GET["number"];
      if ($val) {
        $isexp = $_GET["exp"];
        $args = $val;
        if ($isexp) {
          $args = $args . ' -e';
        }
        $output = '';
        $code = 0;
        $stdout = exec('./train_bin ' . $args, $output, $code);
        if ($code != 0) {
          echo 'Uh oh, something went wrong (' . $code . '): ' . $stdout;
        } else if ($stdout == 1) {
          echo $val . ' works';
          if ($isexp) {
            echo ' (with exponentials... try again <a href="train_numbers.php?number=' . $val . '">without</a>)';
          }
        } else {
          echo $val . " doesn't work";
          if ($isexp) {
            echo ' (even with exponentials)';
          } else {
            echo ' (try again <a href="train_numbers.php?number=' . $val . '&exp=on">with exponentials</a>)';
          }
        }
      }
    ?>
  </body>
</html>
