function drawSpriteHitbox(objDataStart)

  hitBoxOffsetX = emu.read(objDataStart + 12, emu.memType.snesWorkRam, false)
  hitBoxOffsetY = emu.read(objDataStart + 13, emu.memType.snesWorkRam, false)
  hitBoxSizeX = emu.read(objDataStart + 14, emu.memType.snesWorkRam, false)
  hitBoxSizeY = emu.read(objDataStart + 15, emu.memType.snesWorkRam, false)
  
  calHitBoxLeft = emu.read(objDataStart + 16, emu.memType.snesWorkRam, false)
  calHitBoxRight = emu.read(objDataStart + 17, emu.memType.snesWorkRam, false)
  calHitBoxTop = emu.read(objDataStart + 18, emu.memType.snesWorkRam, false)
  calHitBoxBottom = emu.read(objDataStart + 19, emu.memType.snesWorkRam, false)
  
  print(calHitBoxLeft)
  
  screenY = emu.read(objDataStart + 1, emu.memType.snesWorkRam, false)
  screenX = emu.read(objDataStart, emu.memType.snesWorkRam, false)
  
  emu.drawRectangle(screenX + hitBoxOffsetX, screenY + hitBoxOffsetY, hitBoxSizeX, hitBoxSizeY, 0x304040FF, false, 1) 
  emu.drawRectangle(calHitBoxLeft, calHitBoxTop, calHitBoxRight - calHitBoxLeft, calHitBoxBottom - calHitBoxTop, 0x30FF000F, false, 2) 

  emu.drawRectangle(8, 8, 128, 40, 0x304040FF, true, 1)
  emu.drawRectangle(8, 8, 128, 40, 0x304040FF, false, 1)
  if(objDataStart == 0x104) then
	  emu.drawString(12, 12, "Player Left Pos: " .. calHitBoxLeft, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 21, "Player Right Pos: " .. calHitBoxRight, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 30, "Player Top Pos: " .. calHitBoxTop, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 39, "Player Bottom Pos: " .. calHitBoxBottom, 0xFFFFFF, 0xFF000000)
  end
end

function drawAllHitboxes()
	-- player object
	drawSpriteHitbox(0x104)
	-- target object
	drawSpriteHitbox(0x12c)
end

emu.addEventCallback(drawAllHitboxes, emu.eventType.endFrame);