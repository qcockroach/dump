Account = {}
Account.__index = Account;

function Account:create(balance)
	local acnt = {}             -- our new object
	setmetatable(acnt, Account) -- make Account hanle lookup
	acnt.balance = balance      -- initialize our object
	return acnt
end

function Account:withdaw(amount)
	self.balance = self.balance - amount
end


-- create and use Account
acc = Account:create(100);
acc:withdaw(20)

print("Account's balance: "..tostring(acc.balance))


