-- ---------------------------------------------------------------------------
-- vehicle_seat_addon : server-side per-seat orientation + exit overrides
--
-- Ports the long-standing TrinityCore VehicleSeatAddon subsystem to this 8.3
-- fork. With this table present, a seat's passenger facing can be overridden
-- server-side (SeatOrientation, radians) instead of relying only on the client
-- VehicleSeat.db2 PassengerYaw. Core reads it via ObjectMgr::LoadVehicleSeatAddon
-- and applies it in Vehicle.cpp when a passenger boards.
--
-- SeatEntry      : VehicleSeat.db2 id (the per-seat entry, NOT the vehicle id)
-- SeatOrientation: facing offset in radians, relative to the vehicle (0..2*PI)
-- ExitParamX/Y/Z/O + ExitParamValue: optional dismount placement
--                  (0 = none, 1 = offset from vehicle, 2 = absolute destination)
-- ---------------------------------------------------------------------------
DROP TABLE IF EXISTS `vehicle_seat_addon`;
CREATE TABLE `vehicle_seat_addon` (
  `SeatEntry`       INT UNSIGNED   NOT NULL DEFAULT 0,
  `SeatOrientation` FLOAT          NOT NULL DEFAULT 0,
  `ExitParamX`      FLOAT          NOT NULL DEFAULT 0,
  `ExitParamY`      FLOAT          NOT NULL DEFAULT 0,
  `ExitParamZ`      FLOAT          NOT NULL DEFAULT 0,
  `ExitParamO`      FLOAT          NOT NULL DEFAULT 0,
  `ExitParamValue`  TINYINT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`SeatEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Vehicle Seat Addon (per-seat orientation/exit overrides)';

-- No rows by default: the core falls back to VehicleSeat.db2 PassengerYaw for
-- every seat, so behaviour is unchanged until you add an override row here.
-- Example (uncomment + set a real seat id to make a seat face 90 degrees left):
-- INSERT INTO `vehicle_seat_addon`
--   (`SeatEntry`, `SeatOrientation`, `ExitParamX`, `ExitParamY`, `ExitParamZ`, `ExitParamO`, `ExitParamValue`) VALUES
--   (/*seatId*/ 0, 1.5708, 0, 0, 0, 0, 0);
