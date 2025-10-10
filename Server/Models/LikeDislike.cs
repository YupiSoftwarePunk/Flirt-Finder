namespace Server.Models
{
    public class LikeDislike
    {
        public int Id { get; set; }
        public int UserId { get; set; }
        public int LikedBy { get; set; }
        public bool Reaction { get; set; }

        public User User { get; set; }
        public Message Message { get; set; }
    }
}
