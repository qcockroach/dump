phrases = [
	"Hi, Nikki! What can I do for you?",
	"Is there something urgent or just talking to him?",
	"What's message I should deliver to Virm?",
	"Virm's out. Taking shower as I know. Is there I can deliver to Virm?",
	"I made aware Virm about you",
	"Your message is delivered. He'll text you when will be online again. Just be patient",
	"My responses are limited. Please, choose on of options below: opt1, opt2, opt3, ..."
]

def getreply(msg, name="Nikki"):
	print (">> getreply: ", msg)
	Tmsg = msg_prepare(msg)
	return msg_handle(Tmsg)

def msg_prepare(Tmsg):
	Tmsg = Tmsg.lower()
	return Tmsg

def msg_handle(Tmsg):
	if (Tmsg == "hi"):
		print (">> hi")
		return phrases[0]
	else:
		print(">>", Tmsg)

	reply = "Hi, Nikki! I made aware Virm about you. He'll text you when will be online again"
	return reply
