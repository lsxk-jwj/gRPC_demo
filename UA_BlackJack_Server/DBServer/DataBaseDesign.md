### 数据库需要提供的功能
#### 玩家
- 查询昵称
- 查询密码
- 查询UID
- 查询历史记录
- 查询好友列表

#### 游戏
- 查询积分
- 记录历史

#### 社交
- 添加、删除好友
- 添加、删除待接收好友请求

#### 排名
- 更新排名
- 查看排名
- 查看前n名

### 数据库关系规划
- UID -> nickname	实现:(UID:name -> <string> nickname)
- UID -> password	实现:(UID:pass -> <string> password)
- UID -> score		（UID:score -> <string> score )
- UID -> friendList	(UID:friend -> <set> friendList)
- UID -> matchList  (UID:matches -> <set> matchList)
- UID -> waitingFriendList (UID:waitFriend -> <set> friendList)
- nickname -> uid	(nickname:uid -> <string> UID)
- rank -> rankSet	(RANK -> <sorted_set> rank )
- matchID -> match	(MID:match -> <hash> matchinfo)
