itv = {}

function pp(arg)
	print("LUA PRINT --> " .. arg) 
end

function itv:new(o)
	o=o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end

local datasets = {}
datasets[1] = {}
datasets[1][1.25] = {1.65}
datasets[1][1.65] = {2.38}
datasets[1][2.38] = {2.99, 4.94}
datasets[1][2.99] = {3.35}
datasets[1][3.35] = {4.12}
datasets[1][4.12] = {4.94,1.25}
datasets[1][4.94] = {6.02}
datasets[1][6.02] = {6.62}
datasets[1][6.62] = {1.25, 4.12}

datasets[2] = {}
datasets[2][9.5] = {3,10}
datasets[2][3] = {11}
datasets[2][11] = {3, 10}
datasets[2][10] = {9.5, 2}
datasets[2][2] = {11, 9.5}



datasets[3] = {}
datasets[3][2.5] = {9, 2} 
datasets[3][9] = {1}
datasets[3][1] = {9, 2}
datasets[3][2] = {2.5, 10}
datasets[3][10] = {1, 2.5}


datasets[4] = {}
datasets[4][0.25] = {0.66, 1}
datasets[4][0.66] = {0.5, 0.33}
datasets[3][0.33] = {0.66, 0.25}
datasets[4][1] = {0.75, 0.33}
datasets[4][0.75] = {0.25, 1}

function itv:init(dataset, start_val, curval, rangeMin, rangeMax)
	pp("initialize")
	self.data = {}
	self.data = datasets[dataset]

	pp("data ok")
	-- current is interval
	self.current = start_val
	-- self.value is real note
	self.init = curval
	self.value = curval

	self.rangeMin = rangeMin
	self.rangeMax = rangeMax
	pp("method :")

self.methods = {
	--min
	function() 
		local min = nil
		for k,v in pairs(self.data[self.current]) do
			if(min == nil) then
				min = v
			else
				if(v < min) then
					min = v
				end
			end
		end
		
		self.current = min
		return min
	end,

	--max
	function()
		local min = nil
		for k,v in pairs(self.data[self.current]) do
			if(min == nil) then
				min = v
			else
				if(v > min) then
					min = v
				end
			end
		end
		
		self.current = min
		return min


	end,

	--Sorted second
	function()
		local rnd = math.random(2)
		table.sort(self.data[self.current])
		if(#self.data[self.current] >= 2) then
			if(rnd == 1) then
				self.current = self.data[self.current][2]
				return self.data[self.current][2]
			else
				self.current = self.data[self.current][#self.data[self.current]-1]
			end
		else
			self.current = self.data[self.current][1]
			return self.data[self.current][1]
		end
	end,
	--median
	function()
		local size = #self.data[self.current]
		local halfsize = math.floor(size/2)
		self.current = self.data[self.current][halfsize]
		return self.current
	end,

	--rand
	function()
		--math.randomseed(os.time())
		local rnd = math.random(#self.data[self.current])
		self.current = self.data[self.current][rnd]
		return self.current
	end,
	--ascend descend
	function()
		if(self.sign == nil) then self.sign = "+" end
		local rnd = math.random(#self.data[self.current])
		self.current = self.data[self.current][rnd]
		if(self.sign == "+") then
			self.value = self.value + (self.current)
		elseif(self.sign == "-") then
			self.value = self.value + (self.current * -1)
		end
		if(self.value >= self.rangeMax) then self.sign = "-" end
		if(self.value <= self.rangeMin) then self.sign = "+" end

	end,
	--ascend
	function()
		local rnd = math.random(#self.data[self.current])
		self.current = self.data[self.current][rnd]
		self.value = self.value + self.current
		if(self.value > self.rangeMax) then
			local rest = math.abs(self.rangeMax - self.value)
			self.value = self.rangeMin + rest
		end
	end,
	--descend
	function()
		local rnd = math.random(#self.data[self.current])
		self.current = self.data[self.current][rnd]
		self.value = self.value - self.current
		if(self.value < self.rangeMin) then
			local rest = math.abs(self.rangeMin - self.value)
			self.value = self.rangeMax - rest
		end
	end,

}


end

function itv:kperf(ktrig, method_idx)
	if(ktrig ~= 0) then
		index = math.floor(method_idx)
		self.methods[index]()
		--self.method = methods[index]
		--self.method[index](self)
		--[[
		if(self.value >= self.rangeMax) then
			--self.methods[1]()
			local wrap = self.value - self.rangeMax
			self.value = self.init + self.current + wrap
		elseif(self.value <= self.rangeMin) then
			--self.methods[2]()
			local wrap = self.rangeMin - self.value
			self.value = self.init + self.current + wrap
		else
			self.value = self.value + self.current
		end
		]]--

	end

	return self.value
end


