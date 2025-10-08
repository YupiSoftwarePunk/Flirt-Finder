namespace Server.DTOs
{
    public class UserDto
    {
        public string Id { get; set; }
        public string Username { get; set; }
        public string FullName { get; set; }
        public DateTime CreatedAt { get; set; }
        public string Bio { get; internal set; }
        public string Gender { get; internal set; }
        public DateTime? BirthDate { get; internal set; }
        public string PhotoUrl { get; internal set; }
    }
}
