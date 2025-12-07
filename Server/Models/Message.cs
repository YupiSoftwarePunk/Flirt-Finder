using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Server.Models
{
    [Table("messages")]
    public class Message
    {
        [Key]
        [Column("id")]
        public int Id { get; set; }

        [Column("sender_id")]
        public int SenderId { get; set; }

        [Column("receiver_id")]
        public int ReceiverId { get; set; }

        [Column("message_text")]
        public string Content { get; set; }

        [Column("send_time")]
        public DateTime Timestamp { get; set; }
        public bool IsForwarded { get; set; }


        [ForeignKey("SenderId")]
        public User Sender { get; set; }

        [ForeignKey("ReceiverId")]
        public User Receiver { get; set; }
        
    }
}
