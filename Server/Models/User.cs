namespace Server.Models
{
    public class User
    {
        public string Id { get; set; }
        public string Username { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public DateTime? BirthDate { get; set; }
        public string PhotoUrl { get; set; }
        public string PasswordHash { get; set; }
        public string FullName { get; internal set; }
    }
}
