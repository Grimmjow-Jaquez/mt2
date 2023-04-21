# @Grimm
# Generator bosses pos from map, for clientside system information.
if __name__ == "__main__":

	# Config Me..
	szPathMaps = "/usr/main/srv1/share/locale/germany/map/"
	szOutPutBossFile = "/usr/main/srv1/share/locale/grm_work/boss_info"

	def GetMapIndexFromString(string):
		ret = ""
		for strw in string:
			if strw == '\t' or strw == '\n' or strw == '\r':
				continue
				
			ret += str(strw)
		
		if len(ret) == 0:
			return 0
		
		return int(ret)

	def GetValue(string, index_tab):
		indexTabCurrent = 0
		
		ret = ""
		for strw in string:
			if strw == '\t' or strw == '\n' or strw == '\r':
				indexTabCurrent += 1
				continue
				
			if indexTabCurrent == index_tab:
				ret += str(strw)
			
		return ret
		
	def SaveBossInfo(map_index, race, respawn_time, x, y):
		print("Map Index %d, Boss %d, respawn_time %s, x %d, y %d" % (map_index, race, respawn_time, x, y))
		
		fileOpen = open(szOutPutBossFile + "/race_%d.txt" % (race),"a+")
		fileOpen.write("INFO_MOB	%d	%s	%d	%d\n" % (map_index, respawn_time, x, y))
		fileOpen.close()

	# Clear Output Folder
	try:
		import shutil
		shutil.rmtree(szOutPutBossFile)
	except Exception:
		pass
		
	# Create Folder
	import os
	os.makedirs(szOutPutBossFile)
	
	infoMaps = {}

	with open(szPathMaps + "index", "r") as fList:
		wow = fList.read()
		wow = wow.replace("\n", " ")

		infoMaps = wow.split(" ")
	
	# Start search and make in folders
	
	lastMapIndex = 0
	for i, mapInfo in enumerate(infoMaps):
		if i % 2 == 0:
			lastMapIndex = GetMapIndexFromString(mapInfo)
			continue
		
		infoBoss = {}
		try:
			with open(szPathMaps + mapInfo + "/boss.txt", "r") as fList:
				wow = fList.readlines()
				infoBoss = wow
		except Exception:
			pass

		for wText in infoBoss:
			if wText[0] != 'm': # Is not mob, maybe group or smth
				continue
			
			string = wText
			
			values = {}
			for x in range(11):
				value = GetValue(string, x)
				values[x] = value
			
			# Generate Mob Data
			SaveBossInfo(lastMapIndex, int(values[10]), values[7], int(values[1]), int(values[2]))

	print("Generator exit with success!")
