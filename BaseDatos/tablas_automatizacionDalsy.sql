CREATE TABLE Proveedor (
 CIF CHAR(9) PRIMARY KEY,
 Nombre CHAR(50) NOT NULL
);

CREATE TABLE Cliente (
 CIF CHAR(9) PRIMARY KEY,
 Nombre CHAR(50) NOT NULL,
 Ubicacion CHAR(30)
);

CREATE TABLE Empleado (
 DNI CHAR(9) PRIMARY KEY,
 Cod_empl CHAR(9) UNIQUE,
 Nombre CHAR(50) NOT NULL,
 Puesto CHAR(40) NOT NULL
);

CREATE TABLE Incidencia (
 Cod_incidencia CHAR(9) PRIMARY KEY,
 Tipo_incidencia CHAR(40) NOT NULL,
 Tiempo_usado INT NOT NULL
);

CREATE TABLE Encargos (
 Cod_encargo CHAR(10) PRIMARY KEY,
 Fecha_pedido DATE NOT NULL,
 Estado CHAR(15) NOT NULL,
 Precio INT,
 CIF CHAR(9),
 FOREIGN KEY (CIF) REFERENCES Cliente(CIF)
);

CREATE TABLE Lote (
 Cod_Lote CHAR(10) PRIMARY KEY,
 Tamano_max_lote INT NOT NULL,
 Estado CHAR(15) NOT NULL,
 Cod_encargo CHAR(10),
 FOREIGN KEY (Cod_encargo) REFERENCES Encargos(Cod_encargo)
);

CREATE TABLE Pedidos (
 Cod_pedido CHAR(9) PRIMARY KEY,
 Fecha_pedido DATE NOT NULL,
 Fecha_entrega DATE,
 Estado CHAR(20) NOT NULL,
 Cantidad_pedida INT NOT NULL,
 Precio INT,
 CIF CHAR(9),
 FOREIGN KEY (CIF) REFERENCES Proveedor(CIF)
);

CREATE TABLE Botella (
 Cod_bot CHAR(9) PRIMARY KEY,
 Llenada BOOL NOT NULL,
 Taponada BOOL NOT NULL,
 Empaquetada BOOL NOT NULL,
 Cod_pedido CHAR(9),
 Cod_Lote CHAR(10),
 FOREIGN KEY (Cod_pedido) REFERENCES Pedidos(Cod_pedido),
 FOREIGN KEY (Cod_Lote) REFERENCES Lote(Cod_Lote)
);

CREATE TABLE Num_telefono_proveedor (
 CIF CHAR(9),
 Num_telefono CHAR(9),
 PRIMARY KEY (CIF, Num_telefono),
 FOREIGN KEY (CIF) REFERENCES Proveedor(CIF)
);

CREATE TABLE Tipo_material (
 CIF CHAR(9),
 Tipo_material CHAR(30),
 PRIMARY KEY (CIF, Tipo_material),
 FOREIGN KEY (CIF) REFERENCES Proveedor(CIF)
);

CREATE TABLE Telefono_cliente (
 CIF CHAR(9),
 Num_telefono CHAR(9),
 PRIMARY KEY (CIF, Num_telefono),
 FOREIGN KEY (CIF) REFERENCES Cliente(CIF)
);

CREATE TABLE Telefono_empleado (
 DNI CHAR(9),
 Telefono CHAR(9),
 PRIMARY KEY (DNI, Telefono),
 FOREIGN KEY (DNI) REFERENCES Empleado(DNI)
);

CREATE TABLE Direccion_empleado (
 DNI CHAR(9),
 Direccion CHAR(50),
 PRIMARY KEY (DNI, Direccion),
 FOREIGN KEY (DNI) REFERENCES Empleado(DNI)
);

CREATE TABLE Lote_Incidencia (
 Cod_Lote CHAR(10),
 Cod_incidencia CHAR(9),
 PRIMARY KEY (Cod_Lote, Cod_incidencia),
 FOREIGN KEY (Cod_Lote) REFERENCES Lote(Cod_Lote),
 FOREIGN KEY (Cod_incidencia) REFERENCES Incidencia(Cod_incidencia)
);

CREATE TABLE Empleado_Incidencia (
 DNI CHAR(9),
 Cod_incidencia CHAR(9),
 PRIMARY KEY (DNI, Cod_incidencia),
 FOREIGN KEY (DNI) REFERENCES Empleado(DNI),
 FOREIGN KEY (Cod_incidencia) REFERENCES Incidencia(Cod_incidencia)
);

CREATE TABLE Fecha_entrega_encargo (
 Cod_encargo CHAR(10),
 Fecha_entrega DATE,
 PRIMARY KEY (Cod_encargo, Fecha_entrega),
 FOREIGN KEY (Cod_encargo) REFERENCES Encargos(Cod_encargo)
);
