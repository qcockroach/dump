1) NAT or how to connect to your remote server outside of local network
	https://superuser.com/questions/1112193/why-cant-i-access-my-web-server-from-outside-the-network/1112210#1112210

Your home router (or most of the wifi routers) you connect to uses NAT to 
"hide" the subnet behind it and allow your multiple devices communicate on the 
single global IP address you get from the ISP. If you do a request to a remote 
server from LAN, from any device, the remote will see that the request 
originated by your router. If you do a request from outside to your public IP 
address, you actually adressing your router. If you want all HTTP requests that 
addressed to your router be served by your laptop behind it, you have to add a 
port-forward rule in your router's menu to the laptop's IP address and port 80 
(standard HTTP port), or port 443 for HTTPS.

2) vboxnet0, vmnet1, vmnet8 – Virtual machine interface working in bridge mode or NAT mode on Linux.

3) difference between router, modem and gateway
https://medium.com/@AriaZhu/switch-vs-router-vs-modem-what-is-the-difference-598c9426a8b1 (!> the best explaination)
https://www.actiontec.com/wifihelp/what-are-the-differences-between-a-modem-a-router-a-gateway-and-a-modem-router
