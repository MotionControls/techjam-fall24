function drawSpriteHitbox(objDataStart)

  hitBoxOffsetX = emu.read(objDataStart + 12, emu.memType.snesWorkRam, false)
  hitBoxOffsetY = emu.read(objDataStart + 13, emu.memType.snesWorkRam, false)
  hitBoxSizeX = emu.read(objDataStart + 14, emu.memType.snesWorkRam, false)
  hitBoxSizeY = emu.read(objDataStart + 15, emu.memType.snesWorkRam, false)
  
  calHitBoxLeft = emu.read(objDataStart + 16, emu.memType.snesWorkRam, false)
  calHitBoxLeft = calHitBoxLeft | emu.read(objDataStart + 17, emu.memType.snesWorkRam, false) << 8
  calHitBoxRight = emu.read(objDataStart + 18, emu.memType.snesWorkRam, false)
  calHitBoxRight = calHitBoxRight | emu.read(objDataStart + 19, emu.memType.snesWorkRam, false) << 8
  calHitBoxTop = emu.read(objDataStart + 20, emu.memType.snesWorkRam, false)
  calHitBoxTop = calHitBoxTop | emu.read(objDataStart + 21, emu.memType.snesWorkRam, false) << 8
  calHitBoxBottom = emu.read(objDataStart + 22, emu.memType.snesWorkRam, false)
  calHitBoxBottom = calHitBoxBottom | emu.read(objDataStart + 23, emu.memType.snesWorkRam, false) << 8
  
  screenY = emu.read(objDataStart + 1, emu.memType.snesWorkRam, false)
  screenX = emu.read(objDataStart, emu.memType.snesWorkRam, false)
  
  emu.drawRectangle(screenX + hitBoxOffsetX, screenY + hitBoxOffsetY, hitBoxSizeX, hitBoxSizeY, 0x304040FF, false, 1) 
  emu.drawRectangle(calHitBoxLeft, calHitBoxTop, calHitBoxRight - calHitBoxLeft, calHitBoxBottom - calHitBoxTop, 0x30FF000F, false, 2) 
  if(objDataStart == 0x134) then
  	emu.drawRectangle(8, 8, 128, 40, 0x304040FF, true, 1)
  	emu.drawRectangle(8, 8, 128, 40, 0x304040FF, false, 1)
  
	  emu.drawString(12, 12, "Player Left Pos: " .. calHitBoxLeft, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 21, "Player Right Pos: " .. calHitBoxRight, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 30, "Player Top Pos: " .. calHitBoxTop, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 39, "Player Bottom Pos: " .. calHitBoxBottom, 0xFFFFFF, 0xFF000000)
  end
end

function drawAllHitboxes()
	startObjIdx = 0x104
	for i=0,15 do
		objIdx = startObjIdx + (i * 0x30)
		active = emu.read(objIdx + 0x21, emu.memType.snesWorkRam, false)
		if not (active == 255) then
			drawSpriteHitbox(startObjIdx + (i * 0x30))
		end
	end
end

emu.addEventCallback(drawAllHitboxes, emu.eventType.endFrame);