namespace Server.Models
{
    public class User
    {
        public string Username { get; internal set; }
        public object PasswordHash { get; internal set; }
    }
}
