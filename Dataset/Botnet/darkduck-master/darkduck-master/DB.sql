CREATE TABLE `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `password` varchar(255) NOT NULL,
  `duration_limit` int(10) DEFAULT NULL,
  `last_paid` int(10) NOT NULL,
  `max_bots` int(11) DEFAULT '-1',
  `admin` int(10) unsigned DEFAULT '0',
  `intvl` int(10) unsigned DEFAULT '30',
  `api_key` text,
  PRIMARY KEY (`id`),
  KEY `username` (`username`)
);


INSERT INTO users (username, password, duration_limit, admin, last_paid) VALUES ('admin', PASSWORD('admin'), -1, 1, -1);