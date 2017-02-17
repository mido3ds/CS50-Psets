port=`jot -r 1 5000 9000`
php -S localhost:$port & open http://localhost:$port/phpliteadmin.php
