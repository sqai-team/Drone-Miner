create database Ebuhlo;

use Ebuhlo;

#---CREATE SCHEMA IF NOT EXISTS `DB_JDBC` DEFAULT CHARACTER SET utf8;
#USE `DB_JDBC`;


CREATE TABLE loginUsr(
	id int NOT NULL ,
	login varchar (50) NOT NULL ,
	passwrd varchar (50) NOT NULL,
    PRIMARY KEY (id)
) ENGINE = InnoDB;


CREATE TABLE alkohol(
	id int NOT NULL,
	name varchar (50) NOT NULL,
	price float NOT NULL,
	type varchar (50) NOT NULL,
	deskription varchar(100) NOT NULL,
    size float NOT NULL,
    image BLOB NOT NULL,
    PRIMARY KEY (id)
) ENGINE = InnoDB;


CREATE TABLE orders(
	id_order INT AUTO_INCREMENT,
	id_customer int NOT NULL,
    id_alkohol int NOT NULL,
    PRIMARY KEY (id_customer, id_alkohol),
	CONSTRAINT  FOREIGN KEY (id_customer)
		REFERENCES  loginUsr (id),
	CONSTRAINT   FOREIGN KEY (id_alkohol)
		REFERENCES  alkohol (id)
) ENGINE = InnoDB;


INSERT INTO `loginUsr` VALUES 
(1, 'igors', '1234'),
(2, 'maxs', '1234'),
(3, 'sanias', '1234'),
(4, 'bodiam', '1234'),
(5, 'andriyt', '1234');


INSERT INTO `alkohol` VALUES 
(1, 'Cronenburg blanc', '24.90', 'beer', 'Light unfiltered beer. Maker: Carlsberg Ukraine. 4.8%', '0.46', 'D:\EbuhloImages\Beer\blanc.jpeg'),
(2, 'Teteriv', '19', 'beer', 'Light unfiltered beer. Maker: Ukraine. 8%', '0.5', 'D:\EbuhloImages\Beer\teteriv.jpg'),
(3, 'Budweiser', '21.50', 'beer', 'Light filtered beer. Maker: Ukraine. 5%', '0.5', 'D:\EbuhloImages\Beer\bud.jpeg'),
(4, 'Lvivske rizdviane', '15', 'beer', 'Dark unfiltered beer. Maker: Ukraine. 4.4%', '0.5', 'D:\EbuhloImages\Beer\lv_christmas.jpg'),
(5, 'Stella artois', '32', 'beer', 'Light filtered beer. Maker: Ukraine. 4.8%', '0.5', 'D:\EbuhloImages\Beer\stella_artois.jpg'),
(6, 'Finlandia', '210', 'vodka', 'Maker: Finland. 40%', '0.5', 'D:\EbuhloImages\Vodka\finlandia.jpeg'),
(7, 'Zubrovka zlota', '105', 'vodka', 'Maker: Poland. 38%', '0.5', 'D:\EbuhloImages\Vodka\zlota.jpg'),
(8, 'Green day', '85', 'vodka', 'Maker: Ukraine. 40%', '0.5', 'D:\EbuhloImages\Vodka\green_day.jpg'),
(9, 'Nemiroff black', '89', 'vodka', 'Maker: Ukraine. 40%', '0.5', 'D:\EbuhloImages\Vodka\nemiroff.jpg'),
(10, 'Red label', '310', 'whiskey', '6 years of exposure. Maker: Scotland. 40%', '0.5', 'D:\EbuhloImages\Whiskey\red_label.jpg'),
(11, 'Black label', '700', 'whiskey', '12 years of exposure. Maker: Scotland. 40%', '0.5', 'D:\EbuhloImages\Whiskey\black_label.png'),
(12, 'Jack Daniels', '380', 'whiskey', 'Maker: USA. 40%', '0.5', 'D:\EbuhloImages\Whiskey\jack_daniels.png'),
(13, 'Jim Beam', '350', 'whiskey', '4 years of exposure. Maker: USA. 40%', '0.5', 'D:\EbuhloImages\Whiskey\jim_beam.jpg'),
(14, 'Boiarishnik', '10', 'spirt', 'Maker: Russia. 75%', '0.1', 'D:\EbuhloImages\Dessert\boiarka.jpg');


INSERT INTO `orders` VALUES
	(1, 1, 4),
    (2, 1, 2),
    (3, 2, 11),
    (4, 3, 12),
    (5, 4, 14),
    (6, 5, 10);









