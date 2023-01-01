![unit test](https://github.com/amarkelov/community-aid/actions/workflows/unittest.yml/badge.svg)

# Synopsis

The project began as volunteer effort to help local community call centre with their Linux server that run [GoodMorning project](http://www.goodmorningservice.co.uk) system developed in Glasgow I believe.

Later the community guys asked me to make few changes to the look and feel and add few extra features and so it began. Some of this features pushed me into re-developing the system from scratch. I replaced MySQL with PostgreSQL, introduced stored procedures, keys. Added reporting and system configuration interfaces.

More places expressed interest in using the system, so I decided to write down installation procedure and Operator's manual and since the code was open source from the day one, it removed the risk of me being the only person who knew how things work :-)

The installation manual below explains how to do complete system install:

- install Linux server from scratch with software mirroring
- install all the required packages
- install and configure PostgreSQL
- configure iptables
- configure DNS and DHCP services
- install and configure backup system

I tried to make it as detailed as possible.

Unfortunately, I haven't had a chance to update the procedure since Debian 4 (my bad). I'll do my best to fix it as soon as possible. Also, my plan was to package the code and provide simpler installation that the described below. All of this and more will come soon.

Originally the project was hosted on SourceForge. Once they restore the site, it may become available again.

# Installation instructions

**Few notes before we begin**

_The following instructions assume that we install local private network 192.168.0/24 with the server on 192.168.0.1 named *kerry*.
Client machines can be either Windows or Linux/UNIX. In either case I would advise you to use Mozilla/Firefox browser on client machines for better look of the pages. Internet Explorer is OK, but sometimes it does produce slightly different look.
I assume you have at least basic Linux/UNIX skills. Although, I was trying to make it as detailed as possible.
Instructions are based on Debian version 4 release 3, but there is no problem to get the same install on any Linux or BSD UNIX. I'll do instructions for FreeBSD later. Some things are a bit different there (RAID, user names, even directory layout is a bit different)._

## Installing on Debian Linux

### Step 1. OS installation and partitioning

Install minimum system. During the installation create a separate partition with mount point `/srv`. That is where we will hold all the files.

I do tend to install mirrored drives to provide a bit more safety to the system. If you can't get RAID controller, you still can setup software RAID. There is a very good resource to read on the subject of setting up software RAID on Debian - [Emidio Planamente's page on software RAID configuration](http://www.planamente.ch/emidio/pages/linux_howto_root_lvm_raid_etch.php)

I have slightly changed the layout. I created three partitions: one for `/boot `(~200MB), one for `swap` (2x of RAM) and the last one for LVM (the rest of space). LVM would have volume group named rootvg with six logical volumes: `rootlv` (/), `usrlv` (/usr), `varlv` (/var), `tmplv` (/tmp), `homelv` (/home) and `srvlv` (/srv)

Here's how I have it.

This is the content of `/etc/mdadm/mdadm.conf`:

```
# mdadm.conf
#
# Please refer to mdadm.conf(5) for information about this file.
#

# by default, scan all partitions (/proc/partitions) for MD superblocks.
# alternatively, specify devices to scan, using wildcards if desired.
DEVICE partitions

# auto-create devices with Debian standard permissions
CREATE owner=root group=disk mode=0660 auto=yes

# automatically tag new arrays as belonging to the local system
HOMEHOST <system>

# instruct the monitoring daemon where to send mail alerts
MAILADDR root

# definitions of existing MD arrays
ARRAY /dev/md0 level=raid1 num-devices=2 UUID=7c18e811:9712b418:ede9eecf:bba4ae7
ARRAY /dev/md1 level=raid1 num-devices=2 UUID=e450b1ce:24ad7c03:fc369c40:1ef2d5a
ARRAY /dev/md2 level=raid1 num-devices=2 UUID=d7d4c943:023f13b5:b2a26f96:de0ab6e

# This file was auto-generated on Sat, 03 Nov 2007 17:20:31 +0000
# by mkconf $Id: mkconf 261 2006-11-09 13:32:35Z madduck $
```

This is how it looks like in `/proc/mdstat`

```
Personalities : [raid1]
md2 : active raid1 sda3[0] sdb3[1]
      156738048 blocks [2/2] [UU]

md1 : active raid1 sda2[0] sdb2[1]
      3903680 blocks [2/2] [UU]

md0 : active raid1 sda1[0] sdb1[1]
      192640 blocks [2/2] [UU]

unused devices: <none>
```

This is how `/etc/fstab` file looks like :

```
# /etc/fstab: static file system information.
#
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
proc            /proc           proc    defaults        0       0
/dev/mapper/rootvg-rootlv /               reiserfs notail          0       1
/dev/md0        /boot           reiserfs notail          0       2
/dev/mapper/rootvg-homelv /home           reiserfs defaults        0       2
/dev/mapper/rootvg-srvlv /srv            reiserfs defaults        0       2
/dev/mapper/rootvg-tmplv /tmp            reiserfs defaults        0       2
/dev/mapper/rootvg-usrlv /usr            reiserfs defaults        0       2
/dev/mapper/rootvg-varlv /var            reiserfs defaults        0       2
/dev/md1        none            swap    sw              0       0
/dev/hda        /media/cdrom0   udf,iso9660 user,noauto     0       0
```

Here is `df -k` output:

```
Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/mapper/rootvg-rootlv
                        393200     99360    293840  26% /
tmpfs                  1038316         0   1038316   0% /lib/init/rw
udev                     10240        52     10188   1% /dev
tmpfs                  1038316         0   1038316   0% /dev/shm
/dev/md0                192628     40160    152468  21% /boot
/dev/mapper/rootvg-homelv
                        131064     32840     98224  26% /home
/dev/mapper/rootvg-srvlv
                        524268    235196    28907   45% /srv
/dev/mapper/rootvg-tmplv
                       1048540     32844   1015696   4% /tmp
/dev/mapper/rootvg-usrlv
                       1048540    273416    775124  27% /usr
/dev/mapper/rootvg-varlv
                      20970876     54736  20916140   1% /var
```

Give a good slice to `/var`, because that is where PostgreSQL keeps its data directories by default on Debian.

Good practise is to follow 'stable' version of the Operating system for your production servers. So, first thing we do is changing to 'stable' all pointers in `/etc/apt/source.list` file. It will look like this:

```
deb http://ftp.ie.debian.org/debian/ stable main
deb-src http://ftp.ie.debian.org/debian/ stable main

deb http://security.debian.org/ stable/updates main
deb-src http://security.debian.org/ stable/updates main
```

Now we are ready to do upgrade to latest stable version. Login as root and run:

```
bash# apt-get update
bash# apt-get upgrade
```

Next thing to do is to install ssh. I advise you to keep it the only service available to connect to your server (save the Apache server that you need to run the software :-).

To install SSH run:

```
bash# apt-get install ssh
```

Now is the time for the community-aid software installation.

### Step 2. Installing community-aid software

**2.1. Apache configuration**

First of all let's install the basic components we need: Apache, PHP5, PostgreSQL and some extra libraries.

```
bash# apt-get install apache2 php5 libapache2-mod-php5 postgresql-8.1 php5-pgsql cvsnt php-fpdf ssl-cert
```

Now it's the time to pull out the source code from SourceForge. Go to Download page of the project and download the latest release available. Use tar and gzip to unpack the file. The below assumes you have extracted the archive into `/srv` directory.

We have to change some owneship and rights now to enforce security:

```
bash# chmod -R o-rwx /srv/community-aid
bash# chmod o+x /srv/community-aid
bash# chmod o+x /srv/community-aid/conf
bash# chgrp -R www-data /srv/community-aid/php
bash# chgrp -R www-data /srv/community-aid/www
bash# chgrp www-data /srv/community-aid/conf
bash# chgrp www-data /srv/community-aid/conf/community-aid.ini
bash# chmod g+w /srv/community-aid/conf/community-aid.ini
bash# chgrp -R postgres /srv/community-aid/sql
bash# mkdir /srv/community-aid/www/one-off-backups
bash# chmod 0770 /srv/community-aid/www/one-off-backups
bash# chown root:www-data /srv/community-aid/www/one-off-backups
```

Create directory for one-off backups and and fix its rights:

```
bash# mkdir /srv/community-aid/www/one-off-backups
bash# chmod 0770 /srv/community-aid/www/one-off-backups
bash# chown root:www-data /srv/community-aid/www/one-off-backups
```

Copy Apache configuration file and then edit it if neccessary (Hint: you may need to change ServerName to match you Linux server name)

```
bash# cp /srv/community-aid/conf/community-aid /etc/apache2/sites-available/
bash# vi /etc/apache2/sites-available/community-aid
```

You may want to disable the default site and enable the one we just edited:

```
bash# a2dissite (input 000-default)
bash# a2ensite (input community-aid)
```

Edit `/etc/php5/apache2/php.ini` and change the defaults to the following values:

```
include_path = ".:/usr/share/php:/usr/share/fpdf:/srv/community-aid/php"
memory_limit = 128M
max_execution_time = 360
max_input_time = 360
```

**2.1.1. Apache with SSL**

I recommend you to enable SSL on your Apache. At the end of the day you deal with personal data and there is no harm in more security. This step is purely optional and the system will work just as well without SSL. If you do not want to setup SSL, then just restart your Apache and skip to PostgreSQL setup.

```
bash# a2enmod (input ssl)
bash# mkdir /etc/apache2/ssl
```

You need to create SSL certificate for your server now. (NOTE: make sure that server name you enter at this stage is the same as in your Apache config file!)

```
bash# make-ssl-cert /usr/share/ssl-cert/ssleay.cnf /etc/apache2/ssl/apache.pem
```

Edit `/etc/apache2/ports.conf`. Comment the line 'Listen 80' and add 'Listen 443'

```
#Listen 80
Listen 443
```

Now it's time to restart Apache to make the changes effective:

```
bash# /etc/init.d/apache2 restart
```

**2.2. PostgreSQL configuration**

Now we can create the database for the system. You need .sql files located in `/srv/community-aid/sql` directory. It has commands that create the database and create default users and password, grant access, etc. Take a look at the content. Never hurts to know the insides ;-). Besides, you may want to improve it. Feel free to tell me what do you think. Nothing is perfect and there is always space for improvement.

Anyway, let's do it.

First, as PostgreSQL user (usually `postgres`) run `createdb` and `createlang` commands from the shell:

```
bash$ createdb -E utf8 community-aid "Community-aid project database"
bash$ createlang plpgsql community-aid
```

Now, install the database, using the files provided under `/srv/community-aid/sql` subdirectory. Login as PostgreSQL user (usually `postgres`) and run `psql` command:

```
bash$ psql -d community-aid < /srv/community-aid/sql/community-aid-db-schema.sql
bash$ psql -d community-aid < /srv/community-aid/sql/community-aid-users-rights.sql
bash$ psql -d community-aid < /srv/community-aid/sql/community-aid-default-data.sql
```

There is one scheduled task that we will need to run every midnight. To facilitate that we need to alter `pg_hba.conf`. Add the following line to UNIX sockets section:

```
local   community-aid caadmin  trust
```

And add the following cron task to `postgres` user cron list (the task will clean the flag of the first call of the day, so the next morning all clients will get their usual time slot time against their names):

```
0 0 * * * /usr/bin/psql -d community-aid -U caadmin -c "update client_timeslot_call set timeslot_done='f'" > /dev/null 2>&1
```

**2.3. Let's try it now**

By now you are all set to go. Point your browser to https://<server_ip>/community-aid.php and after accepting SSL certificate you must see the login prompt of the system now.

### Step 3. Security

Security is very important when you deal with sensitive data. The system that you are installing is just one of those system that people would like to see as secure as possible. Regardless of the way you deploy the server, it's a good idea to configure a firewall on the system as well. You can use the following start/stop script for `iptables`. I have borrowed the script from Internet a while ago. I will try to find reference to the authors name and post it here.

```
#!/bin/sh

set -e

# Q: How do I get started?
# A: (Did I mention "do not use it" already? Oh well.)
#    1. Setup your normal iptables rules -- firewalling, port forwarding
#       NAT, etc. When everything is configured the way you like, run:
#
#           /etc/init.d/iptables save active
#
#    2. Setup your your inactive firewall rules -- this can be something
#       like clear all rules and set all policy defaults to accept (which
#       can be done with /etc/init.d/iptables clear). When that is ready,
#       save the inactive ruleset:
#
#           /etc/init.d/iptables save inactive
#
#    3. Controlling the script itself is done through runlevels configured
#       with debconf for package installation. Run "dpkg-reconfigure iptables"
#       to enable or disable after installation.
#
# Q: Is that all?
# A: Mostly. You can save additional rulesets and restore them by name. As
#    an example:
#
#       /etc/init.d/iptables save midnight
#       /etc/init.d/iptables load midnight
#
#
#    Autosave only works with start followed by stop.
#
#    Also, take great care with the halt option. It's almost as good as
#    pulling the network cable, except it disrupts localhost too.
#
#    Also, create the /var/lib/iptables and /var/lib/ip6tables dirs
#    as necessary.

# enable ipv6 support
enable_ipv6=false

# set enable_autosave to "true" to autosave the active ruleset
# when going from start to stop
enable_autosave=false

# set enable_save_counters to "true" to save table counters with
# rulesets
enable_save_counters=true

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

initd="$0"
default="$0"

initd_abort () {
  cmd=$1
  shift
  echo "Aborting iptables $cmd: $@."
  echo
  usage
  exit 0
}

initd_have_a_cow_man () {
  for i in $@; do
    if ! command -v "$i" >/dev/null 2>&1; then
      echo "Aborting iptables initd: no $i executable"
      exit 0
    fi
  done
}

initd_clear () {
  rm -f "$autosave"
  echo -n "Clearing ${iptables_command} ruleset: default ACCEPT policy"
  $iptables_save | sed "/-/d;/^#/d;s/DROP/ACCEPT/" | $iptables_restore
  echo "."
}

initd_halt () {
  rm -f $autosave
  echo -n "Clearing ${iptables_command} ruleset: default DROP policy"
  $iptables_save | sed "/-/d;/^#/d;s/ACCEPT/DROP/" | $iptables_restore
  echo "."
}

initd_load () {
  ruleset="$libdir/$@"
  if ! test -f "$ruleset"; then
    initd_abort load "unknown ruleset, \"$@\""
  fi
  if test "$@" = inactive; then
    initd_autosave
  fi
  rm -f "$autosave"
  echo -n "Loading ${iptables_command} ruleset: load \"$@\""
  $iptables_restore < "$ruleset"
  echo "."
}

initd_counters () {
  if test "${enable_save_counters:-false}" = true; then
    echo -n " with counters"
    $iptables_save -c > "$ruleset"
  else
    $iptables_save | sed '/^:/s@\[[0-9]\{1,\}:[0-9]\{1,\}\]@[0:0]@g' > "$ruleset"
  fi
}

initd_save () {
  rm -f $autosave
  ruleset="${libdir}/$@"
  echo -n "Saving ${iptables_command} ruleset: save \"$@\""
   initd_counters
  echo "."
}

initd_autosave () {
  if test -f $autosave -a ${enable_autosave-false} = true; then
    ruleset="${libdir}/active"
    echo -n "Autosaving ${iptables_command} ruleset: save \"active\""
    initd_counters
    echo "."
  fi
}

usage () {
#  current="$(ls -m ${libdir} \
#    | sed 's/ \{0,1\}autosave,\{0,1\} \{0,1\}//')"
cat << END
$initd options:
  start|restart|reload|force-reload
     load the "active" ruleset
  save <ruleset>
     save the current ruleset
  load <ruleset>
     load a ruleset
  stop
     load the "inactive" ruleset
  clear
     remove all rules and user-defined chains, set default policy to ACCEPT
  halt
     remove all rules and user-defined chains, set default policy to DROP

Saved ruleset locations: /var/lib/iptables/ and /var/lib/ip6tables/

Please read: $default

END
}

initd_main () {
  case "$1" in
    start|restart|reload|force-reload)
      initd_load active
      if test ${enable_autosave-false} = true; then
        touch $autosave
      fi
      ;;
    stop)
      initd_load inactive
      ;;
    clear)
      initd_clear
      ;;
    halt)
      initd_halt
      ;;
    save)
      shift
      if test -z "$*"; then
        initd_abort save "no ruleset name given"
      else
        initd_save "$*"
      fi
      ;;
    load)
      shift
      if test -z "$*"; then
        initd_abort load "no ruleset name given"
      else
        initd_load "$*"
      fi
      ;;
    save_active) #legacy option
      initd_save active
      ;;
    save_inactive) #legacy option
      initd_save inactive
      ;;
    *)
      echo "$initd: unknown command: \"$*\""
      usage
      ;;
  esac
}

initd_preload() {
  iptables="/sbin/${iptables_command}"
  iptables_save="${iptables}-save"
  iptables_restore="${iptables}-restore"
  libdir="/var/lib/${iptables_command}"
  autosave="${libdir}/autosave"
  initd_have_a_cow_man "$iptables_save" "$iptables_restore"
  ${iptables_command} -nL >/dev/null
  initd_main $*
}

iptables_command=iptables initd_preload $*
if test "$enable_ipv6" = "true"; then
  iptables_command=ip6tables initd_preload $*
fi

exit 0
```

Place the script in `/etc/init.d` directory and run the following command to create necessary start/stop links on your system and make sure the script is executable:

```
bash# chmod +x /etc/init.d/iptables
bash# update-rc.d iptables defaults 10 90
```

this one will create the following links for you:

```
/etc/rc0.d/K90iptables -> ../init.d/iptables
/etc/rc1.d/K90iptables -> ../init.d/iptables
/etc/rc6.d/K90iptables -> ../init.d/iptables
/etc/rc2.d/S10iptables -> ../init.d/iptables
/etc/rc3.d/S10iptables -> ../init.d/iptables
/etc/rc4.d/S10iptables -> ../init.d/iptables
/etc/rc5.d/S10iptables -> ../init.d/iptables
```

Now create directory `/var/lib/iptables` with two files active and inactive.

```
bash# mkdir -p /var/lib/iptables
```

That is how `/var/lib/iptables/active` looks like:

```
*filter
:INPUT DROP
:FORWARD DROP
:OUTPUT ACCEPT
-A INPUT -s 127.0.0.1 -d 127.0.0.1 -i lo+ -j ACCEPT
-A INPUT -p icmp -m icmp --icmp-type any -j ACCEPT
#
# If by any reason you plan to put this machine to a network
# with access to Internet, the following rules will allow to
# sync time wiht Irish NTP servers.
# ntp-galway.hea.net (140.203.16.5)
# ntp-galway.hea.net (140.203.16.5)
# ntp.cs.tcd.ie (134.226.32.57)
# ntp.maths.tcd.ie (134.226.81.3)
# ntp.tcd.ie (134.226.1.114)
-A INPUT -p udp -m udp -s 140.203.16.5 --sport 123 -j ACCEPT
-A INPUT -p udp -m udp -s 134.226.32.57 --sport 123 -j ACCEPT
-A INPUT -p udp -m udp -s 134.226.81.3 --sport 123 -j ACCEPT
-A INPUT -p udp -m udp -s 134.226.1.114 --sport 123 -j ACCEPT
#
# this one allows your local network to access DNS server
-A INPUT -s 192.168.0.0/24 -p udp -m udp --dport 53 -j ACCEPT
-A INPUT -s 192.168.0.0/24 -p tcp -m tcp --dport 53 -j ACCEPT
-A INPUT -s 192.168.0.0/24 -p udp -m udp --dport 67 -j ACCEPT
# this one for the server to access itself
-A INPUT -s 192.168.0.1 -d 192.168.0.1 -i lo+ -j ACCEPT
#
# this one allows your local network to access NTP server
-A INPUT -s 192.168.0.0/24 -p udp -m udp --dport 123 -j ACCEPT
-A INPUT -p tcp -m tcp --dport 22 --tcp-flags SYN,RST,ACK SYN -j ACCEPT
-A INPUT -p tcp -m tcp --dport 443 --tcp-flags SYN,RST,ACK SYN -j ACCEPT
-A INPUT -p tcp -m state --state RELATED,ESTABLISHED -j ACCEPT
#
# you may want to uncomment the next line when you troubleshoot
# to see what's blocked by the firewall
# -A INPUT -j LOG --log-prefix "rejected: "
COMMIT
```

That is how `/var/lib/iptables/inactive` looks like this:

```
*nat
:PREROUTING ACCEPT
:POSTROUTING ACCEPT
:OUTPUT ACCEPT
COMMIT
# Completed on Fri Jul  8 22:35:16 2005
# Generated by iptables-save v1.2.11 on Fri Jul  8 22:35:16 2005
*filter
:INPUT ACCEPT
:FORWARD ACCEPT
:OUTPUT ACCEPT
COMMIT
```

Now we secure the access to the files and restart the firewall to activate the rules:

```
bash# chmod o-rwx /var/lib/iptables/*
bash# /etc/init.d/iptables reload
```

You may want to read more on iptables and securing Debian in more details here.

### Step 4. Time is important

I would encourage you to setup NTP service on the system and make it the time keeper for your network. Nothing is good enough without correct time.
The following instructions based on the NTP server and client configuration instructions.

Let's install NTP first:

```
bash# apt-get install ntp
```

Create `/etc/ntp.conf` that will contain the following:

```
driftfile /var/lib/ntp/ntp.drift
statsdir /var/log/ntpstats/

statistics loopstats peerstats clockstats
filegen loopstats file loopstats type day enable
filegen peerstats file peerstats type day enable
filegen clockstats file clockstats type day enable

server ntp-galway.hea.net
server ntp.cs.tcd.ie
server ntp.maths.tcd.ie
server ntp.tcd.ie

restrict -4 default kod notrap nomodify nopeer noquery
restrict -6 default kod notrap nomodify nopeer noquery

restrict 127.0.0.1
restrict ::1

restrict 192.168.0.0 mask 255.255.255.0 nomodify notrap

broadcast 192.168.0.255
```

On your client machines create `/etc/ntp.conf` with the following content:

```
# /etc/ntp.conf, configuration for ntpd

driftfile /var/lib/ntp/ntp.drift
statsdir /var/log/ntpstats/
server kerry
```

Now restart ntp on the server and all your clients for the changes to take effect:

```
bash# /etc/init.d/ntp restart
```

If you run `ntpq` on your client you should see something like this:

```
bash$ ntpq
ntpq> pe
     remote           refid      st t when poll reach   delay   offset  jitter
==============================================================================
 kerry.community .INIT.          16 u   59   64    0    0.000    0.000   0.000
```

### Step 5. DNS and DHCP services

You are installing you network that has to be self-sufficient. You don't really want to go to every client machine and setup static IP addresses and keep track of it. Good idea is to setup Dynamic DNS service together with DHCP. That way your client machines (including the new one you add later) will be able to pick up IP addresses from the server. It will make your life that bit easier.
The instructions below based upon the following article [http://www.debian-administration.org/articles/343](http://www.debian-administration.org/articles/343)

Let's install the needed packages first:

```
bash# apt-get install bind9 dhcp3-server dnsutils
```

**5.1 DNS configuration**

Now run `rndc-confgen` command to create new key for DHCP and DNS server to use.
Sample output of the command:

```
# Start of rndc.conf
key "rndc-key" {
        algorithm hmac-md5;
        secret "WQTSFQneCq1l5raKlUFnmg==";
};

options {
        default-key "rndc-key";
        default-server 127.0.0.1;
        default-port 953;
};
# End of rndc.conf

# Use with the following in named.conf, adjusting the allow list as needed:
# key "rndc-key" {
#       algorithm hmac-md5;
#       secret "WQTSFQneCq1l5raKlUFnmg==";
# };
#
# controls {
#       inet 127.0.0.1 port 953
#               allow { 127.0.0.1; } keys { "rndc-key"; };
# };
# End of named.conf
```

So, we take the first paragraph and put it into file called `/etc/bind/rndc.key`, which we will later include into BIND config and reference in DHCP config file.

Here's `/etc/bind/rndc.key` file:

```
key "rndc-key" {
      algorithm hmac-md5;
      secret "WQTSFQneCq1l5raKlUFnmg==";
};
```

Add the following into `/etc/bind/named.conf.local` file:

```
controls {
      inet 127.0.0.1 port 953
              allow { 127.0.0.1; } keys { "rndc-key"; };
};

// Add local zone definitions here.
zone "community-aid.network" {
        type master;
        file "/etc/bind/db.community-aid";
        allow-update { key "rndc-key"; };
        notify yes;
};

zone "0.168.192.in-addr.arpa" {
        type master;
        file "/etc/bind/db.192.168.0";
        allow-update { key "rndc-key"; };
        notify yes;
};

include "/etc/bind/rndc.key";
```

Create `/etc/bind/db.192.168.0` that will contain the following:

```
$TTL 86400
@       IN      SOA     ns.community-aid.network. hostmaster.community-aid.network. (
                        50 ; serial
                        28800 ; refresh
                        7200 ; retry
                        604800 ; expire
                        86400 ; ttl
                        )

@       IN      NS      ns.community-aid.network.

1       IN      PTR     kerry.community-aid.network.
```

Create `/etc/bind/db.community-aid` that will contain the following:

```
$TTL 86400
@       IN      SOA     ns.community-aid.network. hostmaster.community-aid.network. (
                        50 ; serial
                        28800 ; refresh (8 hours)
                        7200 ; retry (2 hours)
                        604800 ; retire (1 week)
                        86400 ; ttl (1 day)
                        )

        IN      NS      ns.community-aid.network.

@       IN      MX      1       mail.community-aid.network.

@       IN      A       192.168.0.1
ns      IN      A       192.168.0.1
mail    IN      A       192.168.0.1
kerry   IN      A       192.168.0.1
```

Make sure that `/etc/bind` directory is writable by group bind. By default it's not. Because we setup dynamic DNS service we have to allow the group to write there.
Run:

```
bash# chmod g+w /etc/bind
```

Change `/etc/network/interfaces`:

```
# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
allow-hotplug eth0
iface eth0 inet static
        address 192.168.0.1
        netmask 255.255.255.0
```

Change `/etc/hosts`:

```
127.0.0.1       localhost
192.168.0.1     kerry kerry.community-aid.network

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
ff02::3 ip6-allhosts
```

Change `/etc/resolv.conf` file to point to the new DNS server and insert the new domain name:

```
domain  community-aid.network
nameserver 192.168.0.1
```

And now restart the bind service:

```
bash# /etc/init.d/bind9 restart
```

**5.2. DHCP configuration**

Replace the default `/etc/dhcp3/dhcpd.conf` with the following:

```
# Basic stuff to name the server and switch on updating
server-identifier           server;
ddns-updates                on;
ddns-update-style           interim;
ddns-domainname             "community-aid.network.";
ddns-rev-domainname         "in-addr.arpa.";
ignore                      client-updates;

# This is the key so that DHCP can authenticate it's self to BIND9
include                     "/etc/bind/rndc.key";

# This is the communication zone
zone community-aid.network. {
    primary 127.0.0.1;
    key rndc-key;
}

# Normal DHCP stuff
option domain-name              "community-aid.network.";
option domain-name-servers      192.168.0.1;
option ntp-servers              192.168.0.1;
option ip-forwarding            off;

default-lease-time              600;
max-lease-time                  7200;
authoritative;

log-facility local7;

subnet 192.168.0.0 netmask 255.255.255.0 {
    range                       192.168.0.100 192.168.0.200;
    option broadcast-address    192.168.0.255;
    option routers              192.168.0.1;
    allow                       unknown-clients;

    zone    0.168.192.in-addr.arpa. {
            primary 192.168.0.1;
            key             "rndc-key";
    }

    zone    localdomain. {
            primary 192.168.0.1;
            key             "rndc-key";
    }
}
```

Add the following to `/etc/syslog.conf`:

```
local7.*    /var/log/dhcp.log
```

Now restart the DHCP server for the changes to take effect:

```
bash# /etc/init.d/dhcp3-server restart
```

### Step 6. Backup

No server is good (or lasts long) without a good backup system in place. I prefer Amanda and here is the brief instructions how to set one up for your server. In this instruction I have server with internal DDS DAT-72 tape drive.

Let's install Amanda packages first (NOTE: `mtx` is not really required unless you have a true tape loader)

```
bash# apt-get install mtx amanda-server amanda-client gnuplot
```

Imagine that you have tape drive whose `tapetype` is not yet listed on Amanda web site. Now you need to identify it. But, before you started `amtapetype`, disable compression:

```
bash# mt -f /dev/nst0 datcompression 0
bash# mt -f /dev/nst0 setblk 0
```

Now we can start `amtapetype`. I had Dell's DAT72 drive, hence the description below:

```
bash# amtapetype -f /dev/nst0 -o -e 36G -t "DELL-DAT72"
It took approximately 2-3 hours before I have got the following description from the command above:

define tapetype DELL-DAT72 {
    comment "just produced by tapetype prog (hardware compression off)"
    length 35756 mbytes
    filemark 0 kbytes
    speed 4651 kps
}
```

Now it's time to come up with configuration file for Amanda. I took the default file and changed few things to fit my setup. It will handle 1 week cycle Monday-Saturday with 15 tapes in rotation.

```
#
# amanda.conf - sample Amanda configuration file.
#
# If your configuration is called, say, "kerrydaily", then this file
# normally goes in /etc/amanda/kerrydaily/amanda.conf.
#
# You need to edit this file to suit your needs.  See the documentation in
# this file, in the "man amanda" man page, in the /usr/share/docs/amanda*
# directories, and on the web at www.amanda.org for more information.
#

org "Friendly Call Service. Kerry."     # your organization name for reports
mailto "root"                   # space separated list of operators at your site
dumpuser "backup"               # the user to run dumps under

inparallel 4            # maximum dumpers that will run in parallel (max 63)
                        # this maximum can be increased at compile-time,
                        # modifying MAX_DUMPERS in server-src/driverio.h
dumporder "sssS"        # specify the priority order of each dumper
                        #   s -> smallest size
                        #   S -> biggest size
                        #   t -> smallest time
                        #   T -> biggest time
                        #   b -> smallest bandwitdh
                        #   B -> biggest bandwitdh
                        # try "BTBTBTBTBTBT" if you are not holding
                        # disk constrained

taperalgo first         # The algorithm used to choose which dump image to send
                        # to the taper.

                        # Possible values:
                        #   [first|firstfit|largest|largestfit|smallest|last]
                        # Default: first.

                        # first         First in - first out.
                        # firstfit      The first dump image that will fit on
                        #               the current tape.
                        # largest       The largest dump image.
                        # largestfit    The largest dump image that will fit on
                        #                the current tape.
                        # smallest      The smallest dump image.
                        # last          Last in - first out.

displayunit "k"         # Possible values: "k|m|g|t"
                        # Default: k.
                        # The unit used to print many numbers.
                        # k=kilo, m=mega, g=giga, t=tera

netusage  600 Kbps      # maximum net bandwidth for Amanda, in KB per sec

dumpcycle 1 weeks       # the number of days in the normal dump cycle
runspercycle 5          # the number of amdump runs in dumpcycle days
                        # (4 weeks * 5 amdump runs per week -- just weekdays)
tapecycle 15 tapes      # the number of tapes in rotation
                        # 4 weeks (dumpcycle) times 5 tapes per week (just
                        # the weekdays) plus a few to handle errors that
                        # need amflush and so we do not overwrite the full
                        # backups performed at the beginning of the previous
                        # cycle

bumpsize 20 Mb          # minimum savings (threshold) to bump level 1 -> 2
bumppercent 20          # minimum savings (threshold) to bump level 1 -> 2
bumpdays 1              # minimum days at each level
bumpmult 4              # threshold = bumpsize * bumpmult^(level-1)

etimeout 300            # number of seconds per filesystem for estimates.
dtimeout 1800           # number of idle seconds before a dump is aborted.

ctimeout 30             # maximum number of seconds that amcheck waits
                        # for each client host
tapebufs 20
runtapes 1              # number of tapes to be used in a single run of amdump
tpchanger "chg-manual"  # the tape-changer glue script
tapedev "/dev/nst0"     # the no-rewind tape device to be used
rawtapedev "/dev/st0"   # the raw device to be used (ftape only)

# If you want Amanda to automatically label any non-Amanda tapes it
# encounters, uncomment the line below. Note that this will ERASE any
# non-Amanda tapes you may have, and may also ERASE any near-failing tapes.
# Use with caution.
## label_new_tapes "KERRYDAILY-%%%"

maxdumpsize -1          # Maximum number of bytes the planner will schedule
                        # for a run (default: runtapes * tape_length).
tapetype DELL-DAT72     # what kind of tape it is (see tapetypes below)
labelstr "^KERRYDAILY-[0-9][0-9]*$"     # label constraint regex: all tapes must
 match

amrecover_do_fsf yes            # amrecover will call amrestore with the
                                # -f flag for faster positioning of the tape.
amrecover_check_label yes       # amrecover will call amrestore with the
                                # -l flag to check the label.
holdingdisk hd1 {
    comment "main holding disk"
    directory "/amanda" # where the holding disk is
    use -100 Mb         # how much space can we use on it
                        # a non-positive value means:
                        #        use all space but that value
    chunksize 1Gb       # size of chunk if you want big dump to be
                        # dumped on multiple files on holding disks
                        #  N Kb/Mb/Gb split images in chunks of size N
                        #             The maximum value should be
                        #             (MAX_FILE_SIZE - 1Mb)
                        #  0          same as INT_MAX bytes
    }

autoflush no     #
        # if autoflush is set to yes, then amdump will schedule all dump on
        # holding disks to be flush to tape during the run.

infofile "/etc/amanda/kerrydaily/curinfo"       # database DIRECTORY
logdir   "/etc/amanda/kerrydaily"               # log directory
indexdir "/etc/amanda/kerrydaily/index"         # index directory

# tapetypes
define tapetype DELL-DAT72 {
    comment "just produced by tapetype prog (hardware compression off)"
    length 35756 mbytes
    filemark 0 kbytes
    speed 4651 kps
}

# dumptypes

define dumptype global {
    comment "Global definitions"
    # This is quite useful for setting global parameters, so you don't have
    # to type them everywhere.  All dumptype definitions in this sample file
    # do include these definitions, either directly or indirectly.
    # There's nothing special about the name `global'; if you create any
    # dumptype that does not contain the word `global' or the name of any
    # other dumptype that contains it, these definitions won't apply.
    # Note that these definitions may be overridden in other
    # dumptypes, if the redefinitions appear *after* the `global'
    # dumptype name.
    # You may want to use this for globally enabling or disabling
    # indexing, recording, etc.  Some examples:
    # index yes
    # record no
    # split_diskbuffer "/raid/amanda"
    # fallback_splitsize 64m
}

define dumptype always-full {
    global
    comment "Full dump of this filesystem always"
    compress none
    priority high
    dumpcycle 0
    exclude list ".exclude.lst"
}

define dumptype always-full-tar {
    global
    program "GNUTAR"
    comment "Full backup with tar of this filesystem always"
    compress none
    priority high
    index
    dumpcycle 0
    exclude list ".exclude.lst"
}

# network interfaces
define interface local {
    comment "a local disk"
    use 1000 kbps
}

define interface le0 {
    comment "10 Mbps ethernet"
    use 400 kbps
}
```

I left most of the original comments in the sample above, so you can make some sense out of it all.
Now is the time to tell Amanda what is it that we want her to backup. Create file called `disklist` that will contain the following:

```
kerry    /root   always-full-tar
kerry    /srv    always-full-tar
kerry    /etc    always-full-tar
kerry    /var    always-full-tar
```

You'd want to create `.exclude.lst` files that you may use to exclude files/directories from backups. I'll use one of the files to exclude MySQL working database directory later. I'm going to dump the database via cron and don't need to touch live files.

```
bash# touch /root/.exclude.lst
bash# touch /etc/.exclude.lst
bash# touch /var/.exclude.lst
bash# touch /srv/.exclude.lst
bash# chown backup:backup /root/.exclude.lst /etc/.exclude.lst /var/.exclude.lst /srv/.exclude.lst
```

Here's how `/srv/.exclude.lst` file may look like if I have decide to exclude directory in `/srv/community-aid/sql` directory from the backups

```
./community-aid/sql/
```

Now as last touch we need to fix ownership on our Amanda configuration directory and files:

```
bash# chown -R backup:backup /etc/amanda/kerrydaily
```

Make user backup member of group `www-data` to allow it backup of the php scripts. Just add the name backup to `www-data` entry in `/etc/group` file:

```
www-data:x:33:backup
```

Now it's time to create the script that will do dump of your PostgreSQL database (data only!).

```
# community-aid-db-backup.sh
#
PATH=/root/bin:/bin:/usr/bin:/usr/sbin
CAHOME=/srv/community-aid
CADB=community-aid
DB_BACKUP_DIR=${CAHOME}/backups
BACKUP_CUR=${DB_BACKUP_DIR}/${CADB}-dataonly.sql
BACKUP_PREV=${BACKUP_CUR}.prev

if [ ! -d ${DB_BACKUP_DIR} ]; then
        mkdir ${DB_BACKUP_DIR}
        chown backup:backup ${DB_BACKUP_DIR}
        chmod g+s,o-rwx ${DB_BACKUP_DIR}
fi
if [ -f ${BACKUP_CUR} ]; then
        mv ${BACKUP_CUR} ${BACKUP_PREV}
fi

pg_dump -a --disable-triggers -U caadmin ${CADB} > ${BACKUP_CUR}
# fixing the rights
chmod o-rwx ${BACKUP_CUR} ${BACKUP_PREV}
```

Let's schedule it to run Monday-Saturday at 15:30. Use crontab -e command when logged in as root. That how the entry will look like:

```
30 15 * * 1-6 /root/bin/community-aid-db-backup.sh
```

Last thing to do is to create the holding space we have specified in `amanda.conf`. I was using LVM when I installed the system. This allows me now to painlessly create the logical volume and the filesystem:

```
bash# lvcreate -L 36G -p rw -n amandalv rootvg
bash# vgchange -a y rootvg
bash# mkfs.reiserfs /dev/rootvg/amandalv
bash# mkdir /amanda
bash# mount /dev/rootvg/amandalv /amanda
bash# chown backup:backup /amanda
```

Now you need to label new tapes to include them into the backup cycle and make recognisable by amanda system.
We need to label tapes now. As the amanda.conf file states the label should be in format `"^KERRYDAILY-[0-9][0-9]*$"`

```
labelstr "^KERRYDAILY-[0-9][0-9]*$"
```

The following `amlabel` commad assumes that you do not have a tape loader (there is no slot number at the end of `amlabel` command). We will create `tapelist` file to store the list of the tapes in rotation. Also you will have to repeat the `amlabel` command for each new tape, giving it names KERRYDAILY-002, KERRYDAILY-003 and so forth. Do not forget to place new tape into the tape drive before you run `amlabel` command.

```
bash# su - backup
backup$ touch /etc/amanda/kerrydaily/tapelist
backup$ amlabel -f kerrydaily KERRYDAILY-001
```

Make sure now that your `/var/backups/.amandahosts` file has the following lines in it:

```
localhost    backup
kerry        backup
```

And finally, create cronjob for backup user to run `amdump` command as often as you need. The below will run it every business day:

```
0 20 * * 1-5 /usr/sbin/amdump kerrydaily
```

Congratulations! Now you all set. Please, send me a note if the instructions are missing something or something is not clear enough and I'll gladly update the text above.

# License

```
Copyright (c) 2007, Community-Aid Project
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Community-Aid Project nor the
      names of contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Community-Aid Project ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Community-Aid Project BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
